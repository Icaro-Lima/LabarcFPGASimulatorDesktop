using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace LabarcFPGASimulatorDesktop
{
    public partial class Form1 : Form
    {
        private PictureBox[] SWI;
        private PictureBox[] SEG;

        public Form1()
        {
            InitializeComponent();

            SWI = new PictureBox[]
            {
                pictureBoxSwi0,
                pictureBoxSwi3,
                pictureBoxSwi6,
                pictureBoxSwi1,
                pictureBoxSwi2,
                pictureBoxSwi4,
                pictureBoxSwi5,
                pictureBoxSwi7
            };
            Array.ForEach(SWI, element => SwiInit(element));

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

        private void SwitchClick_Click(object sender, EventArgs e)
        {
            PictureBox actual = (PictureBox)sender;
            
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
    }
}
