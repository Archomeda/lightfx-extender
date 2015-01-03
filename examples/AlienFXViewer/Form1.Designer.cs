namespace AlienFXViewer
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
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.label8 = new System.Windows.Forms.Label();
            this.labelTiming = new System.Windows.Forms.Label();
            this.labelTick = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.labelVersion = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.labelIsInitialized = new System.Windows.Forms.Label();
            this.panelPrimaryColor1 = new System.Windows.Forms.Panel();
            this.labelInfo1 = new System.Windows.Forms.Label();
            this.labelPrimaryColor1 = new System.Windows.Forms.Label();
            this.comboBoxDevice = new System.Windows.Forms.ComboBox();
            this.tableLayoutPanel3 = new System.Windows.Forms.TableLayoutPanel();
            this.panelPrimaryColor4 = new System.Windows.Forms.Panel();
            this.labelInfo4 = new System.Windows.Forms.Label();
            this.labelPrimaryColor4 = new System.Windows.Forms.Label();
            this.panelPrimaryColor3 = new System.Windows.Forms.Panel();
            this.labelInfo3 = new System.Windows.Forms.Label();
            this.labelPrimaryColor3 = new System.Windows.Forms.Label();
            this.panelPrimaryColor2 = new System.Windows.Forms.Panel();
            this.labelInfo2 = new System.Windows.Forms.Label();
            this.labelPrimaryColor2 = new System.Windows.Forms.Label();
            this.tableLayoutPanel4 = new System.Windows.Forms.TableLayoutPanel();
            this.panelSecondaryColor4 = new System.Windows.Forms.Panel();
            this.labelSecondaryColor4 = new System.Windows.Forms.Label();
            this.panelSecondaryColor3 = new System.Windows.Forms.Panel();
            this.labelSecondaryColor3 = new System.Windows.Forms.Label();
            this.panelSecondaryColor2 = new System.Windows.Forms.Panel();
            this.labelSecondaryColor2 = new System.Windows.Forms.Label();
            this.panelSecondaryColor1 = new System.Windows.Forms.Panel();
            this.labelSecondaryColor1 = new System.Windows.Forms.Label();
            this.labelNumDevices = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.tableLayoutPanel1.SuspendLayout();
            this.panelPrimaryColor1.SuspendLayout();
            this.tableLayoutPanel3.SuspendLayout();
            this.panelPrimaryColor4.SuspendLayout();
            this.panelPrimaryColor3.SuspendLayout();
            this.panelPrimaryColor2.SuspendLayout();
            this.tableLayoutPanel4.SuspendLayout();
            this.panelSecondaryColor4.SuspendLayout();
            this.panelSecondaryColor3.SuspendLayout();
            this.panelSecondaryColor2.SuspendLayout();
            this.panelSecondaryColor1.SuspendLayout();
            this.SuspendLayout();
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 2;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 100F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
            this.tableLayoutPanel1.Controls.Add(this.label4, 0, 4);
            this.tableLayoutPanel1.Controls.Add(this.labelNumDevices, 0, 4);
            this.tableLayoutPanel1.Controls.Add(this.label8, 0, 3);
            this.tableLayoutPanel1.Controls.Add(this.labelTiming, 0, 3);
            this.tableLayoutPanel1.Controls.Add(this.labelTick, 1, 1);
            this.tableLayoutPanel1.Controls.Add(this.label3, 0, 1);
            this.tableLayoutPanel1.Controls.Add(this.labelVersion, 1, 0);
            this.tableLayoutPanel1.Controls.Add(this.label1, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.label5, 0, 2);
            this.tableLayoutPanel1.Controls.Add(this.labelIsInitialized, 1, 2);
            this.tableLayoutPanel1.Location = new System.Drawing.Point(12, 12);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 5;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(291, 101);
            this.tableLayoutPanel1.TabIndex = 0;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(3, 60);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(38, 13);
            this.label8.TabIndex = 7;
            this.label8.Text = "Timing";
            this.label8.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // labelTiming
            // 
            this.labelTiming.AutoSize = true;
            this.labelTiming.Location = new System.Drawing.Point(103, 60);
            this.labelTiming.Name = "labelTiming";
            this.labelTiming.Size = new System.Drawing.Size(16, 13);
            this.labelTiming.TabIndex = 6;
            this.labelTiming.Text = "...";
            // 
            // labelTick
            // 
            this.labelTick.AutoSize = true;
            this.labelTick.Location = new System.Drawing.Point(103, 20);
            this.labelTick.Name = "labelTick";
            this.labelTick.Size = new System.Drawing.Size(16, 13);
            this.labelTick.TabIndex = 3;
            this.labelTick.Text = "...";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(3, 20);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(28, 13);
            this.label3.TabIndex = 2;
            this.label3.Text = "Tick";
            // 
            // labelVersion
            // 
            this.labelVersion.AutoSize = true;
            this.labelVersion.Location = new System.Drawing.Point(103, 0);
            this.labelVersion.Name = "labelVersion";
            this.labelVersion.Size = new System.Drawing.Size(16, 13);
            this.labelVersion.TabIndex = 1;
            this.labelVersion.Text = "...";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(3, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(42, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Version";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(3, 40);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(58, 13);
            this.label5.TabIndex = 4;
            this.label5.Text = "IsInitialized";
            // 
            // labelIsInitialized
            // 
            this.labelIsInitialized.AutoSize = true;
            this.labelIsInitialized.Location = new System.Drawing.Point(103, 40);
            this.labelIsInitialized.Name = "labelIsInitialized";
            this.labelIsInitialized.Size = new System.Drawing.Size(16, 13);
            this.labelIsInitialized.TabIndex = 5;
            this.labelIsInitialized.Text = "...";
            // 
            // panelPrimaryColor1
            // 
            this.panelPrimaryColor1.Controls.Add(this.labelInfo1);
            this.panelPrimaryColor1.Controls.Add(this.labelPrimaryColor1);
            this.panelPrimaryColor1.Location = new System.Drawing.Point(3, 3);
            this.panelPrimaryColor1.Name = "panelPrimaryColor1";
            this.panelPrimaryColor1.Size = new System.Drawing.Size(119, 119);
            this.panelPrimaryColor1.TabIndex = 2;
            // 
            // labelInfo1
            // 
            this.labelInfo1.AutoSize = true;
            this.labelInfo1.Location = new System.Drawing.Point(3, 0);
            this.labelInfo1.Name = "labelInfo1";
            this.labelInfo1.Size = new System.Drawing.Size(25, 13);
            this.labelInfo1.TabIndex = 1;
            this.labelInfo1.Text = "Info";
            // 
            // labelPrimaryColor1
            // 
            this.labelPrimaryColor1.AutoSize = true;
            this.labelPrimaryColor1.Location = new System.Drawing.Point(3, 106);
            this.labelPrimaryColor1.Name = "labelPrimaryColor1";
            this.labelPrimaryColor1.Size = new System.Drawing.Size(77, 13);
            this.labelPrimaryColor1.TabIndex = 0;
            this.labelPrimaryColor1.Text = "Primary Color 1";
            // 
            // comboBoxDevice
            // 
            this.comboBoxDevice.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxDevice.FormattingEnabled = true;
            this.comboBoxDevice.Location = new System.Drawing.Point(317, 12);
            this.comboBoxDevice.Name = "comboBoxDevice";
            this.comboBoxDevice.Size = new System.Drawing.Size(283, 21);
            this.comboBoxDevice.TabIndex = 4;
            this.comboBoxDevice.SelectedIndexChanged += new System.EventHandler(this.comboBoxDevice_SelectedIndexChanged);
            // 
            // tableLayoutPanel3
            // 
            this.tableLayoutPanel3.ColumnCount = 2;
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel3.Controls.Add(this.panelPrimaryColor4, 1, 1);
            this.tableLayoutPanel3.Controls.Add(this.panelPrimaryColor3, 0, 1);
            this.tableLayoutPanel3.Controls.Add(this.panelPrimaryColor2, 1, 0);
            this.tableLayoutPanel3.Controls.Add(this.panelPrimaryColor1, 0, 0);
            this.tableLayoutPanel3.Location = new System.Drawing.Point(32, 128);
            this.tableLayoutPanel3.Name = "tableLayoutPanel3";
            this.tableLayoutPanel3.RowCount = 2;
            this.tableLayoutPanel3.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel3.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel3.Size = new System.Drawing.Size(250, 250);
            this.tableLayoutPanel3.TabIndex = 0;
            // 
            // panelPrimaryColor4
            // 
            this.panelPrimaryColor4.Controls.Add(this.labelInfo4);
            this.panelPrimaryColor4.Controls.Add(this.labelPrimaryColor4);
            this.panelPrimaryColor4.Location = new System.Drawing.Point(128, 128);
            this.panelPrimaryColor4.Name = "panelPrimaryColor4";
            this.panelPrimaryColor4.Size = new System.Drawing.Size(119, 119);
            this.panelPrimaryColor4.TabIndex = 5;
            // 
            // labelInfo4
            // 
            this.labelInfo4.AutoSize = true;
            this.labelInfo4.Location = new System.Drawing.Point(3, 0);
            this.labelInfo4.Name = "labelInfo4";
            this.labelInfo4.Size = new System.Drawing.Size(25, 13);
            this.labelInfo4.TabIndex = 4;
            this.labelInfo4.Text = "Info";
            // 
            // labelPrimaryColor4
            // 
            this.labelPrimaryColor4.AutoSize = true;
            this.labelPrimaryColor4.Location = new System.Drawing.Point(3, 106);
            this.labelPrimaryColor4.Name = "labelPrimaryColor4";
            this.labelPrimaryColor4.Size = new System.Drawing.Size(77, 13);
            this.labelPrimaryColor4.TabIndex = 1;
            this.labelPrimaryColor4.Text = "Primary Color 4";
            // 
            // panelPrimaryColor3
            // 
            this.panelPrimaryColor3.Controls.Add(this.labelInfo3);
            this.panelPrimaryColor3.Controls.Add(this.labelPrimaryColor3);
            this.panelPrimaryColor3.Location = new System.Drawing.Point(3, 128);
            this.panelPrimaryColor3.Name = "panelPrimaryColor3";
            this.panelPrimaryColor3.Size = new System.Drawing.Size(119, 119);
            this.panelPrimaryColor3.TabIndex = 4;
            // 
            // labelInfo3
            // 
            this.labelInfo3.AutoSize = true;
            this.labelInfo3.Location = new System.Drawing.Point(3, 0);
            this.labelInfo3.Name = "labelInfo3";
            this.labelInfo3.Size = new System.Drawing.Size(25, 13);
            this.labelInfo3.TabIndex = 3;
            this.labelInfo3.Text = "Info";
            // 
            // labelPrimaryColor3
            // 
            this.labelPrimaryColor3.AutoSize = true;
            this.labelPrimaryColor3.Location = new System.Drawing.Point(3, 106);
            this.labelPrimaryColor3.Name = "labelPrimaryColor3";
            this.labelPrimaryColor3.Size = new System.Drawing.Size(77, 13);
            this.labelPrimaryColor3.TabIndex = 1;
            this.labelPrimaryColor3.Text = "Primary Color 3";
            // 
            // panelPrimaryColor2
            // 
            this.panelPrimaryColor2.Controls.Add(this.labelInfo2);
            this.panelPrimaryColor2.Controls.Add(this.labelPrimaryColor2);
            this.panelPrimaryColor2.Location = new System.Drawing.Point(128, 3);
            this.panelPrimaryColor2.Name = "panelPrimaryColor2";
            this.panelPrimaryColor2.Size = new System.Drawing.Size(119, 119);
            this.panelPrimaryColor2.TabIndex = 3;
            // 
            // labelInfo2
            // 
            this.labelInfo2.AutoSize = true;
            this.labelInfo2.Location = new System.Drawing.Point(3, 0);
            this.labelInfo2.Name = "labelInfo2";
            this.labelInfo2.Size = new System.Drawing.Size(25, 13);
            this.labelInfo2.TabIndex = 2;
            this.labelInfo2.Text = "Info";
            // 
            // labelPrimaryColor2
            // 
            this.labelPrimaryColor2.AutoSize = true;
            this.labelPrimaryColor2.Location = new System.Drawing.Point(3, 106);
            this.labelPrimaryColor2.Name = "labelPrimaryColor2";
            this.labelPrimaryColor2.Size = new System.Drawing.Size(77, 13);
            this.labelPrimaryColor2.TabIndex = 1;
            this.labelPrimaryColor2.Text = "Primary Color 2";
            // 
            // tableLayoutPanel4
            // 
            this.tableLayoutPanel4.ColumnCount = 2;
            this.tableLayoutPanel4.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel4.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel4.Controls.Add(this.panelSecondaryColor4, 1, 1);
            this.tableLayoutPanel4.Controls.Add(this.panelSecondaryColor3, 0, 1);
            this.tableLayoutPanel4.Controls.Add(this.panelSecondaryColor2, 1, 0);
            this.tableLayoutPanel4.Controls.Add(this.panelSecondaryColor1, 0, 0);
            this.tableLayoutPanel4.Location = new System.Drawing.Point(330, 128);
            this.tableLayoutPanel4.Name = "tableLayoutPanel4";
            this.tableLayoutPanel4.RowCount = 2;
            this.tableLayoutPanel4.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel4.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel4.Size = new System.Drawing.Size(250, 250);
            this.tableLayoutPanel4.TabIndex = 5;
            // 
            // panelSecondaryColor4
            // 
            this.panelSecondaryColor4.Controls.Add(this.labelSecondaryColor4);
            this.panelSecondaryColor4.Location = new System.Drawing.Point(128, 128);
            this.panelSecondaryColor4.Name = "panelSecondaryColor4";
            this.panelSecondaryColor4.Size = new System.Drawing.Size(119, 119);
            this.panelSecondaryColor4.TabIndex = 5;
            // 
            // labelSecondaryColor4
            // 
            this.labelSecondaryColor4.AutoSize = true;
            this.labelSecondaryColor4.Location = new System.Drawing.Point(3, 106);
            this.labelSecondaryColor4.Name = "labelSecondaryColor4";
            this.labelSecondaryColor4.Size = new System.Drawing.Size(94, 13);
            this.labelSecondaryColor4.TabIndex = 3;
            this.labelSecondaryColor4.Text = "Secondary Color 4";
            // 
            // panelSecondaryColor3
            // 
            this.panelSecondaryColor3.Controls.Add(this.labelSecondaryColor3);
            this.panelSecondaryColor3.Location = new System.Drawing.Point(3, 128);
            this.panelSecondaryColor3.Name = "panelSecondaryColor3";
            this.panelSecondaryColor3.Size = new System.Drawing.Size(119, 119);
            this.panelSecondaryColor3.TabIndex = 4;
            // 
            // labelSecondaryColor3
            // 
            this.labelSecondaryColor3.AutoSize = true;
            this.labelSecondaryColor3.Location = new System.Drawing.Point(3, 106);
            this.labelSecondaryColor3.Name = "labelSecondaryColor3";
            this.labelSecondaryColor3.Size = new System.Drawing.Size(94, 13);
            this.labelSecondaryColor3.TabIndex = 3;
            this.labelSecondaryColor3.Text = "Secondary Color 3";
            // 
            // panelSecondaryColor2
            // 
            this.panelSecondaryColor2.Controls.Add(this.labelSecondaryColor2);
            this.panelSecondaryColor2.Location = new System.Drawing.Point(128, 3);
            this.panelSecondaryColor2.Name = "panelSecondaryColor2";
            this.panelSecondaryColor2.Size = new System.Drawing.Size(119, 119);
            this.panelSecondaryColor2.TabIndex = 3;
            // 
            // labelSecondaryColor2
            // 
            this.labelSecondaryColor2.AutoSize = true;
            this.labelSecondaryColor2.Location = new System.Drawing.Point(3, 106);
            this.labelSecondaryColor2.Name = "labelSecondaryColor2";
            this.labelSecondaryColor2.Size = new System.Drawing.Size(94, 13);
            this.labelSecondaryColor2.TabIndex = 2;
            this.labelSecondaryColor2.Text = "Secondary Color 2";
            // 
            // panelSecondaryColor1
            // 
            this.panelSecondaryColor1.Controls.Add(this.labelSecondaryColor1);
            this.panelSecondaryColor1.Location = new System.Drawing.Point(3, 3);
            this.panelSecondaryColor1.Name = "panelSecondaryColor1";
            this.panelSecondaryColor1.Size = new System.Drawing.Size(119, 119);
            this.panelSecondaryColor1.TabIndex = 2;
            // 
            // labelSecondaryColor1
            // 
            this.labelSecondaryColor1.AutoSize = true;
            this.labelSecondaryColor1.Location = new System.Drawing.Point(3, 106);
            this.labelSecondaryColor1.Name = "labelSecondaryColor1";
            this.labelSecondaryColor1.Size = new System.Drawing.Size(94, 13);
            this.labelSecondaryColor1.TabIndex = 1;
            this.labelSecondaryColor1.Text = "Secondary Color 1";
            // 
            // labelNumDevices
            // 
            this.labelNumDevices.AutoSize = true;
            this.labelNumDevices.Location = new System.Drawing.Point(103, 80);
            this.labelNumDevices.Name = "labelNumDevices";
            this.labelNumDevices.Size = new System.Drawing.Size(16, 13);
            this.labelNumDevices.TabIndex = 8;
            this.labelNumDevices.Text = "...";
            this.labelNumDevices.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(3, 80);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(46, 13);
            this.label4.TabIndex = 9;
            this.label4.Text = "Devices";
            this.label4.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.White;
            this.ClientSize = new System.Drawing.Size(611, 394);
            this.Controls.Add(this.tableLayoutPanel4);
            this.Controls.Add(this.tableLayoutPanel3);
            this.Controls.Add(this.comboBoxDevice);
            this.Controls.Add(this.tableLayoutPanel1);
            this.Name = "Form1";
            this.Text = "Form1";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.Load += new System.EventHandler(this.Form1_Load);
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel1.PerformLayout();
            this.panelPrimaryColor1.ResumeLayout(false);
            this.panelPrimaryColor1.PerformLayout();
            this.tableLayoutPanel3.ResumeLayout(false);
            this.panelPrimaryColor4.ResumeLayout(false);
            this.panelPrimaryColor4.PerformLayout();
            this.panelPrimaryColor3.ResumeLayout(false);
            this.panelPrimaryColor3.PerformLayout();
            this.panelPrimaryColor2.ResumeLayout(false);
            this.panelPrimaryColor2.PerformLayout();
            this.tableLayoutPanel4.ResumeLayout(false);
            this.panelSecondaryColor4.ResumeLayout(false);
            this.panelSecondaryColor4.PerformLayout();
            this.panelSecondaryColor3.ResumeLayout(false);
            this.panelSecondaryColor3.PerformLayout();
            this.panelSecondaryColor2.ResumeLayout(false);
            this.panelSecondaryColor2.PerformLayout();
            this.panelSecondaryColor1.ResumeLayout(false);
            this.panelSecondaryColor1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.Label labelTick;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label labelVersion;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label labelTiming;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label labelIsInitialized;
        private System.Windows.Forms.Panel panelPrimaryColor1;
        private System.Windows.Forms.ComboBox comboBoxDevice;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel3;
        private System.Windows.Forms.Panel panelPrimaryColor4;
        private System.Windows.Forms.Panel panelPrimaryColor3;
        private System.Windows.Forms.Panel panelPrimaryColor2;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel4;
        private System.Windows.Forms.Panel panelSecondaryColor4;
        private System.Windows.Forms.Panel panelSecondaryColor3;
        private System.Windows.Forms.Panel panelSecondaryColor2;
        private System.Windows.Forms.Panel panelSecondaryColor1;
        private System.Windows.Forms.Label labelPrimaryColor1;
        private System.Windows.Forms.Label labelPrimaryColor4;
        private System.Windows.Forms.Label labelPrimaryColor3;
        private System.Windows.Forms.Label labelPrimaryColor2;
        private System.Windows.Forms.Label labelSecondaryColor4;
        private System.Windows.Forms.Label labelSecondaryColor3;
        private System.Windows.Forms.Label labelSecondaryColor2;
        private System.Windows.Forms.Label labelSecondaryColor1;
        private System.Windows.Forms.Label labelInfo1;
        private System.Windows.Forms.Label labelInfo4;
        private System.Windows.Forms.Label labelInfo3;
        private System.Windows.Forms.Label labelInfo2;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label labelNumDevices;
    }
}

