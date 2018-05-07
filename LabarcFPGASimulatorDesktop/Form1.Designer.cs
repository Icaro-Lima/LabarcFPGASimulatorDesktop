namespace LabarcFPGASimulatorDesktop
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.textEditorControl1 = new ICSharpCode.TextEditor.TextEditorControl();
            this.pictureBox3 = new System.Windows.Forms.PictureBox();
            this.pictureBoxSwi0 = new System.Windows.Forms.PictureBox();
            this.pictureBoxSwi1 = new System.Windows.Forms.PictureBox();
            this.pictureBoxSwi2 = new System.Windows.Forms.PictureBox();
            this.pictureBoxSwi3 = new System.Windows.Forms.PictureBox();
            this.pictureBoxSwi4 = new System.Windows.Forms.PictureBox();
            this.pictureBoxSwi5 = new System.Windows.Forms.PictureBox();
            this.pictureBoxSwi6 = new System.Windows.Forms.PictureBox();
            this.pictureBoxSwi7 = new System.Windows.Forms.PictureBox();
            this.pictureBoxLed0 = new System.Windows.Forms.PictureBox();
            this.pictureBoxLed1 = new System.Windows.Forms.PictureBox();
            this.pictureBoxLed2 = new System.Windows.Forms.PictureBox();
            this.pictureBoxLed3 = new System.Windows.Forms.PictureBox();
            this.pictureBoxLed4 = new System.Windows.Forms.PictureBox();
            this.pictureBoxLed5 = new System.Windows.Forms.PictureBox();
            this.pictureBoxLed6 = new System.Windows.Forms.PictureBox();
            this.pictureBoxLed7 = new System.Windows.Forms.PictureBox();
            this.pictureBoxSegment7 = new System.Windows.Forms.PictureBox();
            this.pictureBoxSegment5 = new System.Windows.Forms.PictureBox();
            this.pictureBoxSegment3 = new System.Windows.Forms.PictureBox();
            this.pictureBoxSegment6 = new System.Windows.Forms.PictureBox();
            this.pictureBoxSegment0 = new System.Windows.Forms.PictureBox();
            this.pictureBoxSegment4 = new System.Windows.Forms.PictureBox();
            this.pictureBoxSegment2 = new System.Windows.Forms.PictureBox();
            this.pictureBoxSegment1 = new System.Windows.Forms.PictureBox();
            this.pictureBox2 = new System.Windows.Forms.PictureBox();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.buttonDefault = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxSwi0)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxSwi1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxSwi2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxSwi3)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxSwi4)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxSwi5)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxSwi6)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxSwi7)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxLed0)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxLed1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxLed2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxLed3)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxLed4)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxLed5)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxLed6)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxLed7)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxSegment7)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxSegment5)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxSegment3)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxSegment6)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxSegment0)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxSegment4)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxSegment2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxSegment1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.SuspendLayout();
            // 
            // textEditorControl1
            // 
            this.textEditorControl1.IsReadOnly = false;
            this.textEditorControl1.Location = new System.Drawing.Point(660, 12);
            this.textEditorControl1.Name = "textEditorControl1";
            this.textEditorControl1.Size = new System.Drawing.Size(497, 585);
            this.textEditorControl1.TabIndex = 29;
            this.textEditorControl1.Text = "module Main(\r\n  input logic [7:0] SWI,\r\n  output logic [7:0] LED,\r\n  output logic" +
    " [7:0]\r\n SEG\r\n);\r\n\r\n// Seu código aqui:\r\n\r\nendmodule";
            this.textEditorControl1.Load += new System.EventHandler(this.textEditorControl1_Load);
            // 
            // pictureBox3
            // 
            this.pictureBox3.BackColor = System.Drawing.SystemColors.Control;
            this.pictureBox3.Image = global::LabarcFPGASimulatorDesktop.Properties.Resources.segments;
            this.pictureBox3.Location = new System.Drawing.Point(263, 12);
            this.pictureBox3.Name = "pictureBox3";
            this.pictureBox3.Size = new System.Drawing.Size(128, 192);
            this.pictureBox3.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBox3.TabIndex = 3;
            this.pictureBox3.TabStop = false;
            // 
            // pictureBoxSwi0
            // 
            this.pictureBoxSwi0.BackColor = System.Drawing.Color.Transparent;
            this.pictureBoxSwi0.Cursor = System.Windows.Forms.Cursors.Hand;
            this.pictureBoxSwi0.Image = global::LabarcFPGASimulatorDesktop.Properties.Resources.slideswitchOn;
            this.pictureBoxSwi0.Location = new System.Drawing.Point(600, 64);
            this.pictureBoxSwi0.Name = "pictureBoxSwi0";
            this.pictureBoxSwi0.Size = new System.Drawing.Size(33, 96);
            this.pictureBoxSwi0.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBoxSwi0.TabIndex = 28;
            this.pictureBoxSwi0.TabStop = false;
            this.pictureBoxSwi0.Tag = "";
            this.pictureBoxSwi0.Click += new System.EventHandler(this.SwitchClick_Click);
            // 
            // pictureBoxSwi1
            // 
            this.pictureBoxSwi1.BackColor = System.Drawing.Color.Transparent;
            this.pictureBoxSwi1.Cursor = System.Windows.Forms.Cursors.Hand;
            this.pictureBoxSwi1.Image = global::LabarcFPGASimulatorDesktop.Properties.Resources.slideswitchOn;
            this.pictureBoxSwi1.Location = new System.Drawing.Point(536, 64);
            this.pictureBoxSwi1.Name = "pictureBoxSwi1";
            this.pictureBoxSwi1.Size = new System.Drawing.Size(33, 96);
            this.pictureBoxSwi1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBoxSwi1.TabIndex = 27;
            this.pictureBoxSwi1.TabStop = false;
            this.pictureBoxSwi1.Tag = "";
            this.pictureBoxSwi1.Click += new System.EventHandler(this.SwitchClick_Click);
            // 
            // pictureBoxSwi2
            // 
            this.pictureBoxSwi2.BackColor = System.Drawing.Color.Transparent;
            this.pictureBoxSwi2.Cursor = System.Windows.Forms.Cursors.Hand;
            this.pictureBoxSwi2.Image = global::LabarcFPGASimulatorDesktop.Properties.Resources.slideswitchOn;
            this.pictureBoxSwi2.Location = new System.Drawing.Point(472, 64);
            this.pictureBoxSwi2.Name = "pictureBoxSwi2";
            this.pictureBoxSwi2.Size = new System.Drawing.Size(33, 96);
            this.pictureBoxSwi2.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBoxSwi2.TabIndex = 26;
            this.pictureBoxSwi2.TabStop = false;
            this.pictureBoxSwi2.Tag = "";
            this.pictureBoxSwi2.Click += new System.EventHandler(this.SwitchClick_Click);
            // 
            // pictureBoxSwi3
            // 
            this.pictureBoxSwi3.BackColor = System.Drawing.Color.Transparent;
            this.pictureBoxSwi3.Cursor = System.Windows.Forms.Cursors.Hand;
            this.pictureBoxSwi3.Image = global::LabarcFPGASimulatorDesktop.Properties.Resources.slideswitchOn;
            this.pictureBoxSwi3.Location = new System.Drawing.Point(408, 64);
            this.pictureBoxSwi3.Name = "pictureBoxSwi3";
            this.pictureBoxSwi3.Size = new System.Drawing.Size(33, 96);
            this.pictureBoxSwi3.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBoxSwi3.TabIndex = 25;
            this.pictureBoxSwi3.TabStop = false;
            this.pictureBoxSwi3.Tag = "";
            this.pictureBoxSwi3.Click += new System.EventHandler(this.SwitchClick_Click);
            // 
            // pictureBoxSwi4
            // 
            this.pictureBoxSwi4.BackColor = System.Drawing.Color.Transparent;
            this.pictureBoxSwi4.Cursor = System.Windows.Forms.Cursors.Hand;
            this.pictureBoxSwi4.Image = global::LabarcFPGASimulatorDesktop.Properties.Resources.slideswitchOn;
            this.pictureBoxSwi4.Location = new System.Drawing.Point(216, 64);
            this.pictureBoxSwi4.Name = "pictureBoxSwi4";
            this.pictureBoxSwi4.Size = new System.Drawing.Size(33, 96);
            this.pictureBoxSwi4.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBoxSwi4.TabIndex = 24;
            this.pictureBoxSwi4.TabStop = false;
            this.pictureBoxSwi4.Tag = "";
            this.pictureBoxSwi4.Click += new System.EventHandler(this.SwitchClick_Click);
            // 
            // pictureBoxSwi5
            // 
            this.pictureBoxSwi5.BackColor = System.Drawing.Color.Transparent;
            this.pictureBoxSwi5.Cursor = System.Windows.Forms.Cursors.Hand;
            this.pictureBoxSwi5.Image = global::LabarcFPGASimulatorDesktop.Properties.Resources.slideswitchOn;
            this.pictureBoxSwi5.Location = new System.Drawing.Point(152, 64);
            this.pictureBoxSwi5.Name = "pictureBoxSwi5";
            this.pictureBoxSwi5.Size = new System.Drawing.Size(33, 96);
            this.pictureBoxSwi5.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBoxSwi5.TabIndex = 23;
            this.pictureBoxSwi5.TabStop = false;
            this.pictureBoxSwi5.Tag = "";
            this.pictureBoxSwi5.Click += new System.EventHandler(this.SwitchClick_Click);
            // 
            // pictureBoxSwi6
            // 
            this.pictureBoxSwi6.BackColor = System.Drawing.Color.Transparent;
            this.pictureBoxSwi6.Cursor = System.Windows.Forms.Cursors.Hand;
            this.pictureBoxSwi6.Image = global::LabarcFPGASimulatorDesktop.Properties.Resources.slideswitchOn;
            this.pictureBoxSwi6.Location = new System.Drawing.Point(88, 64);
            this.pictureBoxSwi6.Name = "pictureBoxSwi6";
            this.pictureBoxSwi6.Size = new System.Drawing.Size(33, 96);
            this.pictureBoxSwi6.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBoxSwi6.TabIndex = 22;
            this.pictureBoxSwi6.TabStop = false;
            this.pictureBoxSwi6.Tag = "";
            this.pictureBoxSwi6.Click += new System.EventHandler(this.SwitchClick_Click);
            // 
            // pictureBoxSwi7
            // 
            this.pictureBoxSwi7.BackColor = System.Drawing.Color.Transparent;
            this.pictureBoxSwi7.Cursor = System.Windows.Forms.Cursors.Hand;
            this.pictureBoxSwi7.Image = global::LabarcFPGASimulatorDesktop.Properties.Resources.slideswitchOn;
            this.pictureBoxSwi7.Location = new System.Drawing.Point(24, 64);
            this.pictureBoxSwi7.Name = "pictureBoxSwi7";
            this.pictureBoxSwi7.Size = new System.Drawing.Size(33, 96);
            this.pictureBoxSwi7.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBoxSwi7.TabIndex = 21;
            this.pictureBoxSwi7.TabStop = false;
            this.pictureBoxSwi7.Tag = "";
            this.pictureBoxSwi7.Click += new System.EventHandler(this.SwitchClick_Click);
            // 
            // pictureBoxLed0
            // 
            this.pictureBoxLed0.Image = global::LabarcFPGASimulatorDesktop.Properties.Resources.ledOn;
            this.pictureBoxLed0.Location = new System.Drawing.Point(428, 236);
            this.pictureBoxLed0.Name = "pictureBoxLed0";
            this.pictureBoxLed0.Size = new System.Drawing.Size(24, 40);
            this.pictureBoxLed0.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBoxLed0.TabIndex = 20;
            this.pictureBoxLed0.TabStop = false;
            // 
            // pictureBoxLed1
            // 
            this.pictureBoxLed1.Image = global::LabarcFPGASimulatorDesktop.Properties.Resources.ledOn;
            this.pictureBoxLed1.Location = new System.Drawing.Point(396, 236);
            this.pictureBoxLed1.Name = "pictureBoxLed1";
            this.pictureBoxLed1.Size = new System.Drawing.Size(24, 40);
            this.pictureBoxLed1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBoxLed1.TabIndex = 19;
            this.pictureBoxLed1.TabStop = false;
            // 
            // pictureBoxLed2
            // 
            this.pictureBoxLed2.Image = global::LabarcFPGASimulatorDesktop.Properties.Resources.ledOn;
            this.pictureBoxLed2.Location = new System.Drawing.Point(364, 236);
            this.pictureBoxLed2.Name = "pictureBoxLed2";
            this.pictureBoxLed2.Size = new System.Drawing.Size(24, 40);
            this.pictureBoxLed2.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBoxLed2.TabIndex = 18;
            this.pictureBoxLed2.TabStop = false;
            // 
            // pictureBoxLed3
            // 
            this.pictureBoxLed3.Image = global::LabarcFPGASimulatorDesktop.Properties.Resources.ledOn;
            this.pictureBoxLed3.Location = new System.Drawing.Point(332, 236);
            this.pictureBoxLed3.Name = "pictureBoxLed3";
            this.pictureBoxLed3.Size = new System.Drawing.Size(24, 40);
            this.pictureBoxLed3.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBoxLed3.TabIndex = 17;
            this.pictureBoxLed3.TabStop = false;
            // 
            // pictureBoxLed4
            // 
            this.pictureBoxLed4.Image = global::LabarcFPGASimulatorDesktop.Properties.Resources.ledOn;
            this.pictureBoxLed4.Location = new System.Drawing.Point(300, 236);
            this.pictureBoxLed4.Name = "pictureBoxLed4";
            this.pictureBoxLed4.Size = new System.Drawing.Size(24, 40);
            this.pictureBoxLed4.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBoxLed4.TabIndex = 16;
            this.pictureBoxLed4.TabStop = false;
            // 
            // pictureBoxLed5
            // 
            this.pictureBoxLed5.Image = global::LabarcFPGASimulatorDesktop.Properties.Resources.ledOn;
            this.pictureBoxLed5.Location = new System.Drawing.Point(268, 236);
            this.pictureBoxLed5.Name = "pictureBoxLed5";
            this.pictureBoxLed5.Size = new System.Drawing.Size(24, 40);
            this.pictureBoxLed5.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBoxLed5.TabIndex = 15;
            this.pictureBoxLed5.TabStop = false;
            // 
            // pictureBoxLed6
            // 
            this.pictureBoxLed6.Image = global::LabarcFPGASimulatorDesktop.Properties.Resources.ledOn;
            this.pictureBoxLed6.Location = new System.Drawing.Point(236, 236);
            this.pictureBoxLed6.Name = "pictureBoxLed6";
            this.pictureBoxLed6.Size = new System.Drawing.Size(24, 40);
            this.pictureBoxLed6.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBoxLed6.TabIndex = 14;
            this.pictureBoxLed6.TabStop = false;
            // 
            // pictureBoxLed7
            // 
            this.pictureBoxLed7.Image = global::LabarcFPGASimulatorDesktop.Properties.Resources.ledOn;
            this.pictureBoxLed7.Location = new System.Drawing.Point(204, 236);
            this.pictureBoxLed7.Name = "pictureBoxLed7";
            this.pictureBoxLed7.Size = new System.Drawing.Size(24, 40);
            this.pictureBoxLed7.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBoxLed7.TabIndex = 13;
            this.pictureBoxLed7.TabStop = false;
            // 
            // pictureBoxSegment7
            // 
            this.pictureBoxSegment7.BackColor = System.Drawing.Color.Transparent;
            this.pictureBoxSegment7.Image = global::LabarcFPGASimulatorDesktop.Properties.Resources.segmentPointOn;
            this.pictureBoxSegment7.Location = new System.Drawing.Point(361, 155);
            this.pictureBoxSegment7.Name = "pictureBoxSegment7";
            this.pictureBoxSegment7.Size = new System.Drawing.Size(15, 15);
            this.pictureBoxSegment7.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBoxSegment7.TabIndex = 11;
            this.pictureBoxSegment7.TabStop = false;
            this.pictureBoxSegment7.Tag = "p";
            // 
            // pictureBoxSegment5
            // 
            this.pictureBoxSegment5.BackColor = System.Drawing.Color.Transparent;
            this.pictureBoxSegment5.Image = ((System.Drawing.Image)(resources.GetObject("pictureBoxSegment5.Image")));
            this.pictureBoxSegment5.Location = new System.Drawing.Point(287, 54);
            this.pictureBoxSegment5.Name = "pictureBoxSegment5";
            this.pictureBoxSegment5.Size = new System.Drawing.Size(13, 52);
            this.pictureBoxSegment5.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBoxSegment5.TabIndex = 10;
            this.pictureBoxSegment5.TabStop = false;
            this.pictureBoxSegment5.Tag = "v";
            // 
            // pictureBoxSegment3
            // 
            this.pictureBoxSegment3.BackColor = System.Drawing.Color.Transparent;
            this.pictureBoxSegment3.Image = ((System.Drawing.Image)(resources.GetObject("pictureBoxSegment3.Image")));
            this.pictureBoxSegment3.Location = new System.Drawing.Point(296, 156);
            this.pictureBoxSegment3.Name = "pictureBoxSegment3";
            this.pictureBoxSegment3.Size = new System.Drawing.Size(52, 13);
            this.pictureBoxSegment3.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBoxSegment3.TabIndex = 5;
            this.pictureBoxSegment3.TabStop = false;
            this.pictureBoxSegment3.Tag = "h";
            // 
            // pictureBoxSegment6
            // 
            this.pictureBoxSegment6.BackColor = System.Drawing.Color.Transparent;
            this.pictureBoxSegment6.Image = ((System.Drawing.Image)(resources.GetObject("pictureBoxSegment6.Image")));
            this.pictureBoxSegment6.Location = new System.Drawing.Point(296, 101);
            this.pictureBoxSegment6.Name = "pictureBoxSegment6";
            this.pictureBoxSegment6.Size = new System.Drawing.Size(52, 13);
            this.pictureBoxSegment6.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBoxSegment6.TabIndex = 6;
            this.pictureBoxSegment6.TabStop = false;
            this.pictureBoxSegment6.Tag = "h";
            // 
            // pictureBoxSegment0
            // 
            this.pictureBoxSegment0.BackColor = System.Drawing.Color.Transparent;
            this.pictureBoxSegment0.Image = ((System.Drawing.Image)(resources.GetObject("pictureBoxSegment0.Image")));
            this.pictureBoxSegment0.Location = new System.Drawing.Point(296, 46);
            this.pictureBoxSegment0.Name = "pictureBoxSegment0";
            this.pictureBoxSegment0.Size = new System.Drawing.Size(52, 13);
            this.pictureBoxSegment0.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBoxSegment0.TabIndex = 4;
            this.pictureBoxSegment0.TabStop = false;
            this.pictureBoxSegment0.Tag = "h";
            // 
            // pictureBoxSegment4
            // 
            this.pictureBoxSegment4.BackColor = System.Drawing.Color.Transparent;
            this.pictureBoxSegment4.Image = ((System.Drawing.Image)(resources.GetObject("pictureBoxSegment4.Image")));
            this.pictureBoxSegment4.Location = new System.Drawing.Point(287, 109);
            this.pictureBoxSegment4.Name = "pictureBoxSegment4";
            this.pictureBoxSegment4.Size = new System.Drawing.Size(13, 52);
            this.pictureBoxSegment4.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBoxSegment4.TabIndex = 9;
            this.pictureBoxSegment4.TabStop = false;
            this.pictureBoxSegment4.Tag = "v";
            // 
            // pictureBoxSegment2
            // 
            this.pictureBoxSegment2.BackColor = System.Drawing.Color.Transparent;
            this.pictureBoxSegment2.Image = ((System.Drawing.Image)(resources.GetObject("pictureBoxSegment2.Image")));
            this.pictureBoxSegment2.Location = new System.Drawing.Point(344, 109);
            this.pictureBoxSegment2.Name = "pictureBoxSegment2";
            this.pictureBoxSegment2.Size = new System.Drawing.Size(13, 52);
            this.pictureBoxSegment2.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBoxSegment2.TabIndex = 8;
            this.pictureBoxSegment2.TabStop = false;
            this.pictureBoxSegment2.Tag = "v";
            // 
            // pictureBoxSegment1
            // 
            this.pictureBoxSegment1.BackColor = System.Drawing.Color.Transparent;
            this.pictureBoxSegment1.Image = ((System.Drawing.Image)(resources.GetObject("pictureBoxSegment1.Image")));
            this.pictureBoxSegment1.Location = new System.Drawing.Point(344, 54);
            this.pictureBoxSegment1.Name = "pictureBoxSegment1";
            this.pictureBoxSegment1.Size = new System.Drawing.Size(13, 52);
            this.pictureBoxSegment1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBoxSegment1.TabIndex = 7;
            this.pictureBoxSegment1.TabStop = false;
            this.pictureBoxSegment1.Tag = "v";
            // 
            // pictureBox2
            // 
            this.pictureBox2.BackColor = System.Drawing.SystemColors.Control;
            this.pictureBox2.Image = global::LabarcFPGASimulatorDesktop.Properties.Resources.board_background;
            this.pictureBox2.Location = new System.Drawing.Point(8, 8);
            this.pictureBox2.Name = "pictureBox2";
            this.pictureBox2.Size = new System.Drawing.Size(642, 210);
            this.pictureBox2.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBox2.TabIndex = 2;
            this.pictureBox2.TabStop = false;
            // 
            // pictureBox1
            // 
            this.pictureBox1.Image = global::LabarcFPGASimulatorDesktop.Properties.Resources.fpga_background;
            this.pictureBox1.Location = new System.Drawing.Point(-4, 0);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(658, 658);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBox1.TabIndex = 12;
            this.pictureBox1.TabStop = false;
            // 
            // buttonDefault
            // 
            this.buttonDefault.Location = new System.Drawing.Point(660, 603);
            this.buttonDefault.Name = "buttonDefault";
            this.buttonDefault.Size = new System.Drawing.Size(75, 30);
            this.buttonDefault.TabIndex = 30;
            this.buttonDefault.Text = "Resetar";
            this.buttonDefault.UseVisualStyleBackColor = true;
            this.buttonDefault.Click += new System.EventHandler(this.buttonDefault_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoSize = true;
            this.ClientSize = new System.Drawing.Size(1169, 808);
            this.Controls.Add(this.buttonDefault);
            this.Controls.Add(this.textEditorControl1);
            this.Controls.Add(this.pictureBox3);
            this.Controls.Add(this.pictureBoxSwi0);
            this.Controls.Add(this.pictureBoxSwi1);
            this.Controls.Add(this.pictureBoxSwi2);
            this.Controls.Add(this.pictureBoxSwi3);
            this.Controls.Add(this.pictureBoxSwi4);
            this.Controls.Add(this.pictureBoxSwi5);
            this.Controls.Add(this.pictureBoxSwi6);
            this.Controls.Add(this.pictureBoxSwi7);
            this.Controls.Add(this.pictureBoxLed0);
            this.Controls.Add(this.pictureBoxLed1);
            this.Controls.Add(this.pictureBoxLed2);
            this.Controls.Add(this.pictureBoxLed3);
            this.Controls.Add(this.pictureBoxLed4);
            this.Controls.Add(this.pictureBoxLed5);
            this.Controls.Add(this.pictureBoxLed6);
            this.Controls.Add(this.pictureBoxLed7);
            this.Controls.Add(this.pictureBoxSegment7);
            this.Controls.Add(this.pictureBoxSegment5);
            this.Controls.Add(this.pictureBoxSegment3);
            this.Controls.Add(this.pictureBoxSegment6);
            this.Controls.Add(this.pictureBoxSegment0);
            this.Controls.Add(this.pictureBoxSegment4);
            this.Controls.Add(this.pictureBoxSegment2);
            this.Controls.Add(this.pictureBoxSegment1);
            this.Controls.Add(this.pictureBox2);
            this.Controls.Add(this.pictureBox1);
            this.Name = "Form1";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Form1";
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxSwi0)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxSwi1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxSwi2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxSwi3)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxSwi4)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxSwi5)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxSwi6)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxSwi7)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxLed0)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxLed1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxLed2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxLed3)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxLed4)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxLed5)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxLed6)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxLed7)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxSegment7)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxSegment5)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxSegment3)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxSegment6)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxSegment0)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxSegment4)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxSegment2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxSegment1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox pictureBox2;
        private System.Windows.Forms.PictureBox pictureBox3;
        private System.Windows.Forms.PictureBox pictureBoxSegment0;
        private System.Windows.Forms.PictureBox pictureBoxSegment3;
        private System.Windows.Forms.PictureBox pictureBoxSegment6;
        private System.Windows.Forms.PictureBox pictureBoxSegment1;
        private System.Windows.Forms.PictureBox pictureBoxSegment2;
        private System.Windows.Forms.PictureBox pictureBoxSegment4;
        private System.Windows.Forms.PictureBox pictureBoxSegment5;
        private System.Windows.Forms.PictureBox pictureBoxSegment7;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.PictureBox pictureBoxLed7;
        private System.Windows.Forms.PictureBox pictureBoxLed6;
        private System.Windows.Forms.PictureBox pictureBoxLed5;
        private System.Windows.Forms.PictureBox pictureBoxLed4;
        private System.Windows.Forms.PictureBox pictureBoxLed3;
        private System.Windows.Forms.PictureBox pictureBoxLed2;
        private System.Windows.Forms.PictureBox pictureBoxLed1;
        private System.Windows.Forms.PictureBox pictureBoxLed0;
        private System.Windows.Forms.PictureBox pictureBoxSwi7;
        private System.Windows.Forms.PictureBox pictureBoxSwi6;
        private System.Windows.Forms.PictureBox pictureBoxSwi5;
        private System.Windows.Forms.PictureBox pictureBoxSwi4;
        private System.Windows.Forms.PictureBox pictureBoxSwi0;
        private System.Windows.Forms.PictureBox pictureBoxSwi1;
        private System.Windows.Forms.PictureBox pictureBoxSwi2;
        private System.Windows.Forms.PictureBox pictureBoxSwi3;
        private ICSharpCode.TextEditor.TextEditorControl textEditorControl1;
        private System.Windows.Forms.Button buttonDefault;
    }
}

