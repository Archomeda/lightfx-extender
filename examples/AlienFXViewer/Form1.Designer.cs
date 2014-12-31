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
            this.label1 = new System.Windows.Forms.Label();
            this.labelVersion = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.labelTick = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.labelIsInitialized = new System.Windows.Forms.Label();
            this.labelTiming = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.tableLayoutPanel2 = new System.Windows.Forms.TableLayoutPanel();
            this.label9 = new System.Windows.Forms.Label();
            this.labelSecondaryColor = new System.Windows.Forms.Label();
            this.labelActionType = new System.Windows.Forms.Label();
            this.label12 = new System.Windows.Forms.Label();
            this.labelPosition = new System.Windows.Forms.Label();
            this.label14 = new System.Windows.Forms.Label();
            this.label15 = new System.Windows.Forms.Label();
            this.labelPrimaryColor = new System.Windows.Forms.Label();
            this.panelPrimaryColor = new System.Windows.Forms.Panel();
            this.panelSecondaryColor = new System.Windows.Forms.Panel();
            this.tableLayoutPanel1.SuspendLayout();
            this.tableLayoutPanel2.SuspendLayout();
            this.SuspendLayout();
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 2;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 100F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
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
            this.tableLayoutPanel1.RowCount = 4;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(291, 81);
            this.tableLayoutPanel1.TabIndex = 0;
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
            // labelVersion
            // 
            this.labelVersion.AutoSize = true;
            this.labelVersion.Location = new System.Drawing.Point(103, 0);
            this.labelVersion.Name = "labelVersion";
            this.labelVersion.Size = new System.Drawing.Size(16, 13);
            this.labelVersion.TabIndex = 1;
            this.labelVersion.Text = "...";
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
            // labelTick
            // 
            this.labelTick.AutoSize = true;
            this.labelTick.Location = new System.Drawing.Point(103, 20);
            this.labelTick.Name = "labelTick";
            this.labelTick.Size = new System.Drawing.Size(16, 13);
            this.labelTick.TabIndex = 3;
            this.labelTick.Text = "...";
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
            // labelTiming
            // 
            this.labelTiming.AutoSize = true;
            this.labelTiming.Location = new System.Drawing.Point(103, 60);
            this.labelTiming.Name = "labelTiming";
            this.labelTiming.Size = new System.Drawing.Size(16, 13);
            this.labelTiming.TabIndex = 6;
            this.labelTiming.Text = "...";
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
            // tableLayoutPanel2
            // 
            this.tableLayoutPanel2.ColumnCount = 2;
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 100F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
            this.tableLayoutPanel2.Controls.Add(this.label9, 0, 3);
            this.tableLayoutPanel2.Controls.Add(this.labelSecondaryColor, 0, 3);
            this.tableLayoutPanel2.Controls.Add(this.labelActionType, 1, 1);
            this.tableLayoutPanel2.Controls.Add(this.label12, 0, 1);
            this.tableLayoutPanel2.Controls.Add(this.labelPosition, 1, 0);
            this.tableLayoutPanel2.Controls.Add(this.label14, 0, 0);
            this.tableLayoutPanel2.Controls.Add(this.label15, 0, 2);
            this.tableLayoutPanel2.Controls.Add(this.labelPrimaryColor, 1, 2);
            this.tableLayoutPanel2.Location = new System.Drawing.Point(309, 12);
            this.tableLayoutPanel2.Name = "tableLayoutPanel2";
            this.tableLayoutPanel2.RowCount = 4;
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel2.Size = new System.Drawing.Size(291, 81);
            this.tableLayoutPanel2.TabIndex = 1;
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(3, 60);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(82, 13);
            this.label9.TabIndex = 7;
            this.label9.Text = "SecondaryColor";
            this.label9.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // labelSecondaryColor
            // 
            this.labelSecondaryColor.AutoSize = true;
            this.labelSecondaryColor.Location = new System.Drawing.Point(103, 60);
            this.labelSecondaryColor.Name = "labelSecondaryColor";
            this.labelSecondaryColor.Size = new System.Drawing.Size(16, 13);
            this.labelSecondaryColor.TabIndex = 6;
            this.labelSecondaryColor.Text = "...";
            // 
            // labelActionType
            // 
            this.labelActionType.AutoSize = true;
            this.labelActionType.Location = new System.Drawing.Point(103, 20);
            this.labelActionType.Name = "labelActionType";
            this.labelActionType.Size = new System.Drawing.Size(16, 13);
            this.labelActionType.TabIndex = 3;
            this.labelActionType.Text = "...";
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(3, 20);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(61, 13);
            this.label12.TabIndex = 2;
            this.label12.Text = "ActionType";
            // 
            // labelPosition
            // 
            this.labelPosition.AutoSize = true;
            this.labelPosition.Location = new System.Drawing.Point(103, 0);
            this.labelPosition.Name = "labelPosition";
            this.labelPosition.Size = new System.Drawing.Size(16, 13);
            this.labelPosition.TabIndex = 1;
            this.labelPosition.Text = "...";
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Location = new System.Drawing.Point(3, 0);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(44, 13);
            this.label14.TabIndex = 0;
            this.label14.Text = "Position";
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.Location = new System.Drawing.Point(3, 40);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(65, 13);
            this.label15.TabIndex = 4;
            this.label15.Text = "PrimaryColor";
            // 
            // labelPrimaryColor
            // 
            this.labelPrimaryColor.AutoSize = true;
            this.labelPrimaryColor.Location = new System.Drawing.Point(103, 40);
            this.labelPrimaryColor.Name = "labelPrimaryColor";
            this.labelPrimaryColor.Size = new System.Drawing.Size(16, 13);
            this.labelPrimaryColor.TabIndex = 5;
            this.labelPrimaryColor.Text = "...";
            // 
            // panelPrimaryColor
            // 
            this.panelPrimaryColor.Location = new System.Drawing.Point(32, 119);
            this.panelPrimaryColor.Name = "panelPrimaryColor";
            this.panelPrimaryColor.Size = new System.Drawing.Size(250, 250);
            this.panelPrimaryColor.TabIndex = 2;
            // 
            // panelSecondaryColor
            // 
            this.panelSecondaryColor.Location = new System.Drawing.Point(330, 119);
            this.panelSecondaryColor.Name = "panelSecondaryColor";
            this.panelSecondaryColor.Size = new System.Drawing.Size(250, 250);
            this.panelSecondaryColor.TabIndex = 3;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(623, 408);
            this.Controls.Add(this.panelSecondaryColor);
            this.Controls.Add(this.panelPrimaryColor);
            this.Controls.Add(this.tableLayoutPanel2);
            this.Controls.Add(this.tableLayoutPanel1);
            this.Name = "Form1";
            this.Text = "Form1";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.Load += new System.EventHandler(this.Form1_Load);
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel1.PerformLayout();
            this.tableLayoutPanel2.ResumeLayout(false);
            this.tableLayoutPanel2.PerformLayout();
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
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel2;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label labelSecondaryColor;
        private System.Windows.Forms.Label labelActionType;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.Label labelPosition;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.Label labelPrimaryColor;
        private System.Windows.Forms.Panel panelPrimaryColor;
        private System.Windows.Forms.Panel panelSecondaryColor;
    }
}

