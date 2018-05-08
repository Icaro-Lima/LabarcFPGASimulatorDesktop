using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using ICSharpCode.TextEditor.Document;

namespace LabarcFPGASimulatorDesktop
{
    public partial class Form1 : Form
    {
        private PictureBox[] SWI;
        private PictureBox[] LED;
        private PictureBox[] SEG;

        private string defaultVerilogTemplate;
        private SaveFileDialog saveFileDialog;
        private OpenFileDialog openFileDialog;

        private Process MainVerilogProcess;

        private enum LogType { Normal = 0, GoodNews = 1, Warning = 2, Error = 3 };

        private int clock;

        public Form1()
        {
            InitializeComponent();

            SWI = new PictureBox[]
            {
                pictureBoxSwi0,
                pictureBoxSwi1,
                pictureBoxSwi2,
                pictureBoxSwi3,
                pictureBoxSwi4,
                pictureBoxSwi5,
                pictureBoxSwi6,
                pictureBoxSwi7
            };
            Array.ForEach(SWI, element => SwiInit(element));

            LED = new PictureBox[]
            {
                pictureBoxLed0,
                pictureBoxLed1,
                pictureBoxLed2,
                pictureBoxLed3,
                pictureBoxLed4,
                pictureBoxLed5,
                pictureBoxLed6,
                pictureBoxLed7
            };

            SEG = new PictureBox[]
            {
                pictureBoxSegment0,
                pictureBoxSegment3,
                pictureBoxSegment6,
                pictureBoxSegment1,
                pictureBoxSegment2,
                pictureBoxSegment4,
                pictureBoxSegment5,
                pictureBoxSegment7
            };
            Array.ForEach(SEG, element => SegmentInit(element));
            Array.Sort(SEG, (a, b) => a.Name.CompareTo(b.Name));

            defaultVerilogTemplate = textEditorControl1.Text;
            saveFileDialog = new SaveFileDialog();
            openFileDialog = new OpenFileDialog();

            CheckGPP();
            CheckVerilator();

            System.Timers.Timer timer = new System.Timers.Timer(500);
            timer.Elapsed += Timer_Elapsed;
            timer.Start();
        }

        private void SwiInit(PictureBox pictureBoxSwi)
        {
            pictureBoxSwi.Tag = true;

            var pos = PointToScreen(pictureBoxSwi.Location);
            pos = pictureBox2.PointToClient(pos);
            pictureBoxSwi.Parent = pictureBox2;
            pictureBoxSwi.Location = pos;
        }

        private void SegmentInit(PictureBox pictureBoxSegment)
        {
            var pos = PointToScreen(pictureBoxSegment.Location);
            pos = pictureBox3.PointToClient(pos);
            pictureBoxSegment.Parent = pictureBox3;
            pictureBoxSegment.Location = pos;
        }

        private void Log(string text, LogType type, bool endl = true)
        {
            Color color = Color.Purple;

            switch ((int)type)
            {
                case 0:
                    color = Color.White;
                    break;
                case 1:
                    color = Color.Green;
                    break;
                case 2:
                    color = Color.Yellow;
                    break;
                case 3:
                    color = Color.Red;
                    break;
            }

            if (endl)
            {
                text += Environment.NewLine;
            }

            richTextBoxConsoleLog.SelectionStart = richTextBoxConsoleLog.TextLength;
            richTextBoxConsoleLog.SelectionLength = 0;

            richTextBoxConsoleLog.SelectionColor = color;
            richTextBoxConsoleLog.AppendText(text);
            richTextBoxConsoleLog.SelectionColor = richTextBoxConsoleLog.ForeColor;

            richTextBoxConsoleLog.ScrollToCaret();
        }

        private void CheckGPP()
        {
            try
            {
                ProcessStartInfo processStartInfo = new ProcessStartInfo("g++", "--version")
                {
                    WindowStyle = ProcessWindowStyle.Hidden,
                    CreateNoWindow = true,
                    RedirectStandardOutput = true,
                    UseShellExecute = false
                };
                Process p = Process.Start(processStartInfo);
                if (p.WaitForExit(1000))
                {
                    Log("Comando g++ bem configurado. [Conjectura]", LogType.GoodNews);
                }
                else
                {
                    Log("O comando g++ demorou mais de 1s para executar, pode ter algo bem errado.", LogType.Error);
                }
                p.Close();
            }
            catch
            {
                Log("Você precisa configurar o g++ nas suas variáveis de ambiente.", LogType.Error);
            }
        }

        private void CheckVerilator()
        {
            try
            {
                ProcessStartInfo processStartInfo = new ProcessStartInfo("verilator", "--version")
                {
                    WindowStyle = ProcessWindowStyle.Hidden,
                    CreateNoWindow = true,
                    RedirectStandardOutput = true,
                    UseShellExecute = false
                };
                Process p = Process.Start(processStartInfo);
                if (p.WaitForExit(1000))
                {
                    Log("Verilator bem configurado. [Conjectura]", LogType.GoodNews);
                }
                else
                {
                    Log("O verilator demorou mais de 1s para executar, pode ter algo bem errado.", LogType.Error);
                }
                p.Close();
            }
            catch
            {
                Log("Você precisa configurar o verilator nas suas variáveis de ambiente.", LogType.Error);
            }
        }

        private void SetLEDState(int led, bool state)
        {
            LED[led].Image = state ? Properties.Resources.ledOn : Properties.Resources.ledOff;
        }

        private void SetSEGState(int seg, bool state)
        {
            if ((string)(SEG[seg].Tag) == "h")
            {
                SEG[seg].Image = state ? Properties.Resources.segmentHorizontalOn : Properties.Resources.segmentHorizontalOff;
            }
            else if ((string)(SEG[seg].Tag) == "v")
            {
                SEG[seg].Image = state ? Properties.Resources.segmentVerticalOn : Properties.Resources.segmentVerticalOff;
            }
            else
            {
                SEG[seg].Image = state ? Properties.Resources.segmentPointOn : Properties.Resources.segmentPointOff;
            }
        }

        private void SwitchClick_Click(object sender, EventArgs e)
        {
            PictureBox actual = (PictureBox)sender;

            int index = actual.Name[actual.Name.Length - 1] - 48;

            if ((bool)actual.Tag)
            {
                actual.Image = Properties.Resources.slideswitchOff;
            }
            else
            {
                actual.Image = Properties.Resources.slideswitchOn;
            }
            actual.Tag = !(bool)actual.Tag;

            SendStatesToVerilog();
        }

        private void TextEditorControl1_Load(object sender, EventArgs e)
        {
            FileSyntaxModeProvider fileSyntaxModeProvider = new FileSyntaxModeProvider(Application.StartupPath);
            HighlightingManager.Manager.AddSyntaxModeFileProvider(fileSyntaxModeProvider);
            textEditorControl1.SetHighlighting("Verilog");
        }

        /// <summary>
        /// Salva o código escrito pelo usuário.
        /// </summary>
        /// <returns>Retorna true caso o usuário escolha um nome de arquivo válido.</returns>
        private bool SaveCode()
        {
            saveFileDialog.Filter = "SystemVerilog|*.sv|Verilog|*.v||*.*";
            if (saveFileDialog.ShowDialog() == DialogResult.OK)
            {
                using (System.IO.StreamWriter writer = new System.IO.StreamWriter(saveFileDialog.FileName, false))
                {
                    writer.WriteLine(textEditorControl1.Text);
                    writer.Close();
                }

                Log("Código salvo em: " + saveFileDialog.FileName, LogType.Normal);
                return true;
            }

            Log("Não foi possível salvar, o usuário cancelou a ação.", LogType.Warning);
            return false;
        }

        private void ButtonReset_Click(object sender, EventArgs e)
        {
            DialogResult result = MessageBox.Show("Deseja salvar antes?", "", MessageBoxButtons.YesNoCancel, MessageBoxIcon.Asterisk, MessageBoxDefaultButton.Button1);
            if (result == DialogResult.Yes)
            {
                if (SaveCode())
                {
                    textEditorControl1.Text = defaultVerilogTemplate;

                    Log("Código salvo e resetado para o padrão.", LogType.Normal);
                }
            }
            else if (result == DialogResult.No)
            {
                textEditorControl1.Text = defaultVerilogTemplate;

                Log("Código resetado para o padrão sem salvar.", LogType.Warning);
            }
            else
            {
                Log("Não foi possível resetar, o usuário cancelou a ação.", LogType.Warning);
            }
        }

        private void LoadCode()
        {
            openFileDialog.Filter = "SystemVerilog|*sv|Verilog|*v||*.*";
            DialogResult result = openFileDialog.ShowDialog();

            if (result == DialogResult.OK)
            {
                using (System.IO.StreamReader reader = new System.IO.StreamReader(openFileDialog.FileName))
                {
                    textEditorControl1.Text = reader.ReadToEnd();
                    reader.Close();
                }

                Log("Código carregado a partir de : " + openFileDialog.FileName, LogType.Normal);
            }
            else
            {
                Log("Não foi possível carregar, o usuário cancelou a ação.", LogType.Warning);
            }
        }

        private void ButtonLoadCode_Click(object sender, EventArgs e)
        {
            DialogResult result = MessageBox.Show("Gostaria de salvar o código atual antes?", "", MessageBoxButtons.YesNoCancel, MessageBoxIcon.Asterisk, MessageBoxDefaultButton.Button1);
            if (result == DialogResult.Yes)
            {
                if (SaveCode())
                {
                    LoadCode();
                }
            }
            else if (result == DialogResult.No)
            {
                LoadCode();
            }
            else
            {
                Log("Não foi possível carregar, o usuário cancelou a ação.", LogType.Warning);
            }
        }

        private void ButtonBuildAndRun_Click(object sender, EventArgs e)
        {
            try
            {
                if (MainVerilogProcess != null)
                {
                    Log("Matando processo atual...", LogType.Normal);
                    pictureBox2.Enabled = false;
                    MainVerilogProcess.CancelOutputRead();
                    MainVerilogProcess.Kill();
                    MainVerilogProcess.Close();
                    MainVerilogProcess = null;
                }

                Log("Copiando o código para um arquivo temporário Main.sv...", LogType.Normal);
                System.IO.File.WriteAllText("Main.sv", textEditorControl1.Text);

                if (System.IO.Directory.Exists("obj_dir"))
                {
                    System.IO.Directory.Delete("obj_dir", true);
                }

                Log("Compilando Main.sv em cabeçalhos do C++...", LogType.Normal);
                ProcessStartInfo verilatorProcessStartInfo = new ProcessStartInfo("verilator", "--cc Main.sv")
                {
                    WindowStyle = ProcessWindowStyle.Hidden,
                    CreateNoWindow = true,
                    RedirectStandardOutput = true,
                    RedirectStandardError = true,
                    UseShellExecute = false
                };
                Process verilatorProcess = Process.Start(verilatorProcessStartInfo);

                if (!verilatorProcess.WaitForExit(10000))
                {
                    Log("Por algum motivo o verilator demorou muito(10000) para responder.", LogType.Error);
                    return;
                }

                Log("Processo do verilator finalizado...", LogType.Normal);

                if (!System.IO.Directory.Exists("obj_dir"))
                {
                    Log(verilatorProcess.StandardError.ReadToEnd(), LogType.Error);
                    return;
                }
                Log("A compilação do Main.sv foi um sucesso!", LogType.GoodNews);

                /*Process[] mainProcesses = Process.GetProcessesByName("Main");
                for (int i = 0; i < mainProcesses.Length; i++)
                {
                    if (System.IO.Path.GetDirectoryName(mainProcesses[i].MainModule.FileName) == Application.StartupPath)
                    {
                        mainProcesses[i].Kill();
                    }
                }*/

                if (System.IO.File.Exists("Main.exe"))
                {
                    System.IO.File.Delete("Main.exe");
                }

                Log("Compilando Main.cpp", LogType.Normal);
                ProcessStartInfo gppProcessStartInfo = new ProcessStartInfo("g++", "-I\"obj_dir\" -I\"include\" obj_dir\\VMain__Syms.cpp include\\verilated.cpp obj_dir\\VMain.cpp Main.cpp -o Main.exe")
                {
                    WindowStyle = ProcessWindowStyle.Hidden,
                    CreateNoWindow = true,
                    RedirectStandardOutput = true,
                    RedirectStandardError = true,
                    UseShellExecute = false
                };
                Process gppProcess = Process.Start(gppProcessStartInfo);

                if (!gppProcess.WaitForExit(10000))
                {
                    Log("Por algum motivo o g++ demorou muito(10000) para responder.", LogType.Error);
                    return;
                }

                if (!System.IO.File.Exists("Main.exe"))
                {
                    Log(gppProcess.StandardError.ReadToEnd(), LogType.Error);
                    return;
                }

                Log("Compilação do Main.cpp concluida!", LogType.GoodNews);

                ProcessStartInfo mainProcessStartInfo = new ProcessStartInfo("Main.exe")
                {
                    WindowStyle = ProcessWindowStyle.Hidden,
                    CreateNoWindow = true,
                    RedirectStandardOutput = true,
                    RedirectStandardInput = true,
                    UseShellExecute = false
                };
                MainVerilogProcess = new Process
                {
                    StartInfo = mainProcessStartInfo
                };
                MainVerilogProcess.OutputDataReceived += MainVerilogProcess_OutputDataReceived;

                MainVerilogProcess.Start();

                MainVerilogProcess.BeginOutputReadLine();

                SendStatesToVerilog();

                pictureBox2.Enabled = true;
            }
            catch (Exception ex)
            {
                Log(ex.Message, LogType.Error);
            }
        }

        private void MainVerilogProcess_OutputDataReceived(object sender, DataReceivedEventArgs e)
        {
            for (int i = 0; i < 8; i++)
            {
                SetLEDState(i, (e.Data[i] - 48) == 1 ? true : false);
            }
            for (int i = 0; i < 8; i++)
            {
                SetSEGState(i, (e.Data[i + 8] - 48) == 1 ? true : false);
            }
        }

        private void Timer_Elapsed(object sender, System.Timers.ElapsedEventArgs e)
        {
            clock = clock == 1 ? 0 : 1;
            SendStatesToVerilog();
        }

        private void SendStatesToVerilog()
        {
            if (MainVerilogProcess == null)
            {
                return;
            }

            string con = "";
            for (int i = 0; i < 8; i++)
            {
                con += (bool)(SWI[i].Tag) ? 1 : 0;
            }
            MainVerilogProcess.StandardInput.WriteLine(con + clock);
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            if (MainVerilogProcess != null)
            {
                MainVerilogProcess.Kill();
            }
        }
    }
}
