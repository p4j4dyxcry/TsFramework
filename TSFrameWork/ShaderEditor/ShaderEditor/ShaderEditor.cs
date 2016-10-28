using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ShaderEditor
{
    public partial class ShaderEditor : Form
    {
        MainMenu mainMenu;
        RichTextBox textbox;

        public ShaderEditor()
        {
            InitializeComponent();
            InitializeMenu();
            InitializeTextEditor();

        }

        // メニューの初期化
        public void InitializeMenu()
        {
            mainMenu = new MainMenu();
            mainMenu.MenuItems.AddRange(new MenuItem[] { new MenuItem("ファイル"), 
                                                         new MenuItem("編集")});
            this.Menu = mainMenu;
        }

        public void InitializeTextEditor()
        {
            string keyword = "int";

            textbox = new RichTextBox();
            textbox.Parent = this;
            textbox.WordWrap = true;
            textbox.Multiline = true;
            textbox.Size = new Size(256, 256);
            textbox.ScrollBars = RichTextBoxScrollBars.Vertical;
            textbox.BackColor = Color.Black;
            textbox.ForeColor = Color.White;
        }

        void onClick(object sender, EventArgs e)
        {
            MessageBox.Show(((MenuItem)sender).Text + "が選択されました", "Menu選択");
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }
    }
}
