using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
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

            CheckVerilator();
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

                textBoxLogs.AppendText("Código salvo em: " + saveFileDialog.FileName + Environment.NewLine);
                return true;
            }

            textBoxLogs.AppendText("Não foi possível salvar, o usuário cancelou a ação." + Environment.NewLine);
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
                    textBoxLogs.AppendText("Código salvo e resetado para o padrão." + Environment.NewLine);
                }
            }
            else if (result == DialogResult.No)
            {
                textEditorControl1.Text = defaultVerilogTemplate;
                textBoxLogs.AppendText("Código resetado para o padrão sem salvar." + Environment.NewLine);
            }
            else
            {
                textBoxLogs.AppendText("Não foi possível resetar, o usuário cancelou a ação." + Environment.NewLine);
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

                textBoxLogs.AppendText("Código carregado a partir de : " + openFileDialog.FileName + Environment.NewLine);
            }
            else
            {
                textBoxLogs.AppendText("Não foi possível carregar, o usuário cancelou a ação." + Environment.NewLine);
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
                textBoxLogs.AppendText("Não foi possível carregar, o usuário cancelou a ação." + Environment.NewLine);
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
                    textBoxLogs.AppendText("Olá, detectei que você configurou corretamente o verilator, posso estar errado, mas não fui configurado para refletir sobre." + Environment.NewLine);
                    textBoxLogs.AppendText(Environment.NewLine + p.StandardOutput.ReadToEnd() + Environment.NewLine + Environment.NewLine);
                }
                else
                {
                    textBoxLogs.AppendText("O verilator demorou mais de 1s para executar, pode ter algo bem errado." + Environment.NewLine);
                }
                p.Close();
            }
            catch
            {
                textBoxLogs.AppendText("Você precisa configurar o verilator nas suas variáveis de ambiente." + Environment.NewLine);
            }
        }

        private void ButtonBuildAndRun_Click(object sender, EventArgs e)
        {

        }
    }
}
