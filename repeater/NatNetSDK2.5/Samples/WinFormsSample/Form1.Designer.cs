namespace WinFormTestApp
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
            this.components = new System.ComponentModel.Container();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle1 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle2 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle3 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle4 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle5 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle6 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle7 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea1 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend1 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.Series series1 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataVisualization.Charting.Title title1 = new System.Windows.Forms.DataVisualization.Charting.Title();
            this.dataGridView1 = new System.Windows.Forms.DataGridView();
            this.ID = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.X = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Y = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Z = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Yaw = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Pitch = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Roll = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.listView1 = new System.Windows.Forms.ListView();
            this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.checkBoxConnect = new System.Windows.Forms.CheckBox();
            this.buttonGetDataDescriptions = new System.Windows.Forms.Button();
            this.chart1 = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.label1 = new System.Windows.Forms.Label();
            this.UpdateUITimer = new System.Windows.Forms.Timer(this.components);
            this.RecordButton = new System.Windows.Forms.Button();
            this.TimelineStopButton = new System.Windows.Forms.Button();
            this.LiveModeButton = new System.Windows.Forms.Button();
            this.TimelinePlayButton = new System.Windows.Forms.Button();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.comboBoxLocal = new System.Windows.Forms.ComboBox();
            this.label3 = new System.Windows.Forms.Label();
            this.Local = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.textBoxServer = new System.Windows.Forms.TextBox();
            this.RadioUnicast = new System.Windows.Forms.RadioButton();
            this.RadioMulticast = new System.Windows.Forms.RadioButton();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.SetPlaybackTakeButton = new System.Windows.Forms.Button();
            this.PlaybackTakeNameText = new System.Windows.Forms.TextBox();
            this.StopRecordButton = new System.Windows.Forms.Button();
            this.SetRecordingTakeButton = new System.Windows.Forms.Button();
            this.RecordingTakeNameText = new System.Windows.Forms.TextBox();
            this.EditModeButton = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart1)).BeginInit();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.SuspendLayout();
            // 
            // dataGridView1
            // 
            this.dataGridView1.AllowUserToAddRows = false;
            this.dataGridView1.AllowUserToDeleteRows = false;
            this.dataGridView1.AllowUserToResizeRows = false;
            this.dataGridView1.AutoSizeColumnsMode = System.Windows.Forms.DataGridViewAutoSizeColumnsMode.Fill;
            dataGridViewCellStyle1.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
            dataGridViewCellStyle1.BackColor = System.Drawing.Color.Silver;
            dataGridViewCellStyle1.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            dataGridViewCellStyle1.ForeColor = System.Drawing.SystemColors.WindowText;
            dataGridViewCellStyle1.SelectionBackColor = System.Drawing.SystemColors.Highlight;
            dataGridViewCellStyle1.SelectionForeColor = System.Drawing.SystemColors.HighlightText;
            dataGridViewCellStyle1.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
            this.dataGridView1.ColumnHeadersDefaultCellStyle = dataGridViewCellStyle1;
            this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView1.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.ID,
            this.X,
            this.Y,
            this.Z,
            this.Yaw,
            this.Pitch,
            this.Roll});
            this.dataGridView1.EnableHeadersVisualStyles = false;
            this.dataGridView1.Location = new System.Drawing.Point(12, 12);
            this.dataGridView1.Name = "dataGridView1";
            this.dataGridView1.ReadOnly = true;
            this.dataGridView1.RowHeadersVisible = false;
            this.dataGridView1.RowTemplate.ReadOnly = true;
            this.dataGridView1.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.CellSelect;
            this.dataGridView1.Size = new System.Drawing.Size(560, 345);
            this.dataGridView1.TabIndex = 1;
            // 
            // ID
            // 
            this.ID.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
            this.ID.FillWeight = 62.07922F;
            this.ID.HeaderText = "ID";
            this.ID.MinimumWidth = 50;
            this.ID.Name = "ID";
            this.ID.ReadOnly = true;
            this.ID.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // X
            // 
            dataGridViewCellStyle2.Format = "N4";
            dataGridViewCellStyle2.NullValue = null;
            this.X.DefaultCellStyle = dataGridViewCellStyle2;
            this.X.FillWeight = 22.52875F;
            this.X.HeaderText = "X";
            this.X.Name = "X";
            this.X.ReadOnly = true;
            this.X.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // Y
            // 
            dataGridViewCellStyle3.Format = "N2";
            dataGridViewCellStyle3.NullValue = null;
            this.Y.DefaultCellStyle = dataGridViewCellStyle3;
            this.Y.FillWeight = 22.52875F;
            this.Y.HeaderText = "Y";
            this.Y.Name = "Y";
            this.Y.ReadOnly = true;
            this.Y.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // Z
            // 
            dataGridViewCellStyle4.Format = "N2";
            dataGridViewCellStyle4.NullValue = null;
            this.Z.DefaultCellStyle = dataGridViewCellStyle4;
            this.Z.FillWeight = 22.52875F;
            this.Z.HeaderText = "Z";
            this.Z.Name = "Z";
            this.Z.ReadOnly = true;
            this.Z.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // Yaw
            // 
            dataGridViewCellStyle5.Format = "N2";
            dataGridViewCellStyle5.NullValue = null;
            this.Yaw.DefaultCellStyle = dataGridViewCellStyle5;
            this.Yaw.FillWeight = 22.52875F;
            this.Yaw.HeaderText = "Yaw (Y)";
            this.Yaw.Name = "Yaw";
            this.Yaw.ReadOnly = true;
            this.Yaw.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // Pitch
            // 
            dataGridViewCellStyle6.Format = "N2";
            dataGridViewCellStyle6.NullValue = null;
            this.Pitch.DefaultCellStyle = dataGridViewCellStyle6;
            this.Pitch.FillWeight = 22.52875F;
            this.Pitch.HeaderText = "Pitch (X)";
            this.Pitch.Name = "Pitch";
            this.Pitch.ReadOnly = true;
            this.Pitch.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // Roll
            // 
            dataGridViewCellStyle7.Format = "N2";
            dataGridViewCellStyle7.NullValue = null;
            this.Roll.DefaultCellStyle = dataGridViewCellStyle7;
            this.Roll.FillWeight = 22.52875F;
            this.Roll.HeaderText = "Roll (Z)";
            this.Roll.Name = "Roll";
            this.Roll.ReadOnly = true;
            this.Roll.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // listView1
            // 
            this.listView1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.listView1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.listView1.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2});
            this.listView1.GridLines = true;
            this.listView1.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.None;
            this.listView1.Location = new System.Drawing.Point(578, 225);
            this.listView1.Name = "listView1";
            this.listView1.Size = new System.Drawing.Size(363, 132);
            this.listView1.TabIndex = 3;
            this.listView1.UseCompatibleStateImageBehavior = false;
            this.listView1.View = System.Windows.Forms.View.Details;
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "Time";
            this.columnHeader1.Width = 80;
            // 
            // columnHeader2
            // 
            this.columnHeader2.Text = "Message";
            this.columnHeader2.Width = 400;
            // 
            // checkBoxConnect
            // 
            this.checkBoxConnect.Appearance = System.Windows.Forms.Appearance.Button;
            this.checkBoxConnect.FlatAppearance.CheckedBackColor = System.Drawing.Color.Red;
            this.checkBoxConnect.FlatAppearance.MouseDownBackColor = System.Drawing.Color.White;
            this.checkBoxConnect.Location = new System.Drawing.Point(12, 102);
            this.checkBoxConnect.Name = "checkBoxConnect";
            this.checkBoxConnect.Size = new System.Drawing.Size(80, 23);
            this.checkBoxConnect.TabIndex = 5;
            this.checkBoxConnect.Text = "Connect";
            this.checkBoxConnect.UseVisualStyleBackColor = true;
            this.checkBoxConnect.CheckedChanged += new System.EventHandler(this.checkBoxConnect_CheckedChanged);
            // 
            // buttonGetDataDescriptions
            // 
            this.buttonGetDataDescriptions.Location = new System.Drawing.Point(110, 103);
            this.buttonGetDataDescriptions.Name = "buttonGetDataDescriptions";
            this.buttonGetDataDescriptions.Size = new System.Drawing.Size(122, 23);
            this.buttonGetDataDescriptions.TabIndex = 11;
            this.buttonGetDataDescriptions.Text = "Get Data Descriptions";
            this.buttonGetDataDescriptions.UseVisualStyleBackColor = true;
            this.buttonGetDataDescriptions.Click += new System.EventHandler(this.buttonGetDataDescriptions_Click);
            // 
            // chart1
            // 
            this.chart1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.chart1.BackColor = System.Drawing.SystemColors.AppWorkspace;
            this.chart1.BackGradientStyle = System.Windows.Forms.DataVisualization.Charting.GradientStyle.TopBottom;
            this.chart1.BackSecondaryColor = System.Drawing.SystemColors.ButtonHighlight;
            this.chart1.BorderlineColor = System.Drawing.Color.Black;
            this.chart1.BorderlineDashStyle = System.Windows.Forms.DataVisualization.Charting.ChartDashStyle.Solid;
            this.chart1.BorderSkin.BackGradientStyle = System.Windows.Forms.DataVisualization.Charting.GradientStyle.TopBottom;
            this.chart1.BorderSkin.BorderDashStyle = System.Windows.Forms.DataVisualization.Charting.ChartDashStyle.Solid;
            this.chart1.BorderSkin.BorderWidth = 5;
            this.chart1.BorderSkin.PageColor = System.Drawing.SystemColors.ButtonFace;
            chartArea1.AxisX.MinorTickMark.Enabled = true;
            chartArea1.AxisX.Title = "Frame";
            chartArea1.AxisY.IsLabelAutoFit = false;
            chartArea1.AxisY.MajorGrid.LineDashStyle = System.Windows.Forms.DataVisualization.Charting.ChartDashStyle.Dash;
            chartArea1.BackColor = System.Drawing.Color.LightGray;
            chartArea1.BackGradientStyle = System.Windows.Forms.DataVisualization.Charting.GradientStyle.TopBottom;
            chartArea1.Name = "ChartArea1";
            this.chart1.ChartAreas.Add(chartArea1);
            legend1.BackColor = System.Drawing.Color.Transparent;
            legend1.BorderColor = System.Drawing.Color.Transparent;
            legend1.DockedToChartArea = "ChartArea1";
            legend1.Name = "Legend1";
            legend1.Position.Auto = false;
            legend1.Position.Height = 10F;
            legend1.Position.Width = 11.48825F;
            legend1.Position.X = 84.04308F;
            legend1.Position.Y = 4F;
            this.chart1.Legends.Add(legend1);
            this.chart1.Location = new System.Drawing.Point(12, 363);
            this.chart1.Name = "chart1";
            series1.ChartArea = "ChartArea1";
            series1.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.FastLine;
            series1.IsValueShownAsLabel = true;
            series1.Label = "value";
            series1.LabelToolTip = "value";
            series1.Legend = "Legend1";
            series1.MarkerBorderColor = System.Drawing.Color.Transparent;
            series1.MarkerColor = System.Drawing.Color.Black;
            series1.MarkerStyle = System.Windows.Forms.DataVisualization.Charting.MarkerStyle.Circle;
            series1.Name = "Series1";
            this.chart1.Series.Add(series1);
            this.chart1.Size = new System.Drawing.Size(929, 289);
            this.chart1.TabIndex = 12;
            this.chart1.Text = "chart1";
            title1.Font = new System.Drawing.Font("Segoe UI", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            title1.Name = "NatNetData";
            title1.Text = "NatNet Demo";
            title1.TextStyle = System.Windows.Forms.DataVisualization.Charting.TextStyle.Shadow;
            this.chart1.Titles.Add(title1);
            // 
            // label1
            // 
            this.label1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.label1.BackColor = System.Drawing.Color.DarkGray;
            this.label1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.ForeColor = System.Drawing.SystemColors.ControlText;
            this.label1.Location = new System.Drawing.Point(578, 201);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(363, 21);
            this.label1.TabIndex = 13;
            this.label1.Text = "Messages";
            // 
            // UpdateUITimer
            // 
            this.UpdateUITimer.Enabled = true;
            this.UpdateUITimer.Interval = 10;
            this.UpdateUITimer.Tick += new System.EventHandler(this.UpdateUITimer_Tick);
            // 
            // RecordButton
            // 
            this.RecordButton.Location = new System.Drawing.Point(11, 69);
            this.RecordButton.Name = "RecordButton";
            this.RecordButton.Size = new System.Drawing.Size(99, 23);
            this.RecordButton.TabIndex = 16;
            this.RecordButton.Text = "Record";
            this.RecordButton.UseVisualStyleBackColor = true;
            this.RecordButton.Click += new System.EventHandler(this.RecordButton_Click);
            // 
            // TimelineStopButton
            // 
            this.TimelineStopButton.Location = new System.Drawing.Point(122, 131);
            this.TimelineStopButton.Name = "TimelineStopButton";
            this.TimelineStopButton.Size = new System.Drawing.Size(99, 23);
            this.TimelineStopButton.TabIndex = 17;
            this.TimelineStopButton.Text = "Timeline Stop";
            this.TimelineStopButton.UseVisualStyleBackColor = true;
            this.TimelineStopButton.Click += new System.EventHandler(this.TimelineStopButton_Click);
            // 
            // LiveModeButton
            // 
            this.LiveModeButton.Location = new System.Drawing.Point(11, 100);
            this.LiveModeButton.Name = "LiveModeButton";
            this.LiveModeButton.Size = new System.Drawing.Size(99, 23);
            this.LiveModeButton.TabIndex = 18;
            this.LiveModeButton.Text = "Live Mode";
            this.LiveModeButton.UseVisualStyleBackColor = true;
            this.LiveModeButton.Click += new System.EventHandler(this.LiveModeButton_Click);
            // 
            // TimelinePlayButton
            // 
            this.TimelinePlayButton.Location = new System.Drawing.Point(11, 131);
            this.TimelinePlayButton.Name = "TimelinePlayButton";
            this.TimelinePlayButton.Size = new System.Drawing.Size(99, 23);
            this.TimelinePlayButton.TabIndex = 20;
            this.TimelinePlayButton.Text = "Timeline Play";
            this.TimelinePlayButton.UseVisualStyleBackColor = true;
            this.TimelinePlayButton.Click += new System.EventHandler(this.TimelinePlayButton_Click);
            // 
            // tabControl1
            // 
            this.tabControl1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Location = new System.Drawing.Point(578, 12);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(363, 186);
            this.tabControl1.TabIndex = 21;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.comboBoxLocal);
            this.tabPage1.Controls.Add(this.label3);
            this.tabPage1.Controls.Add(this.checkBoxConnect);
            this.tabPage1.Controls.Add(this.Local);
            this.tabPage1.Controls.Add(this.label2);
            this.tabPage1.Controls.Add(this.textBoxServer);
            this.tabPage1.Controls.Add(this.RadioUnicast);
            this.tabPage1.Controls.Add(this.RadioMulticast);
            this.tabPage1.Controls.Add(this.buttonGetDataDescriptions);
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(355, 160);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "Connect";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // comboBoxLocal
            // 
            this.comboBoxLocal.FormattingEnabled = true;
            this.comboBoxLocal.Location = new System.Drawing.Point(58, 15);
            this.comboBoxLocal.Name = "comboBoxLocal";
            this.comboBoxLocal.Size = new System.Drawing.Size(121, 21);
            this.comboBoxLocal.TabIndex = 17;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(9, 75);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(31, 13);
            this.label3.TabIndex = 16;
            this.label3.Text = "Type";
            // 
            // Local
            // 
            this.Local.AutoSize = true;
            this.Local.Location = new System.Drawing.Point(9, 18);
            this.Local.Name = "Local";
            this.Local.Size = new System.Drawing.Size(33, 13);
            this.Local.TabIndex = 9;
            this.Local.Text = "Local";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(9, 48);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(38, 13);
            this.label2.TabIndex = 10;
            this.label2.Text = "Server";
            // 
            // textBoxServer
            // 
            this.textBoxServer.Location = new System.Drawing.Point(58, 45);
            this.textBoxServer.Name = "textBoxServer";
            this.textBoxServer.Size = new System.Drawing.Size(121, 20);
            this.textBoxServer.TabIndex = 8;
            this.textBoxServer.Text = "127.0.0.1";
            // 
            // RadioUnicast
            // 
            this.RadioUnicast.AutoSize = true;
            this.RadioUnicast.Location = new System.Drawing.Point(130, 73);
            this.RadioUnicast.Name = "RadioUnicast";
            this.RadioUnicast.Size = new System.Drawing.Size(61, 17);
            this.RadioUnicast.TabIndex = 15;
            this.RadioUnicast.Text = "Unicast";
            this.RadioUnicast.UseVisualStyleBackColor = true;
            this.RadioUnicast.CheckedChanged += new System.EventHandler(this.RadioUnicast_CheckedChanged);
            // 
            // RadioMulticast
            // 
            this.RadioMulticast.AutoSize = true;
            this.RadioMulticast.Checked = true;
            this.RadioMulticast.Location = new System.Drawing.Point(58, 73);
            this.RadioMulticast.Name = "RadioMulticast";
            this.RadioMulticast.Size = new System.Drawing.Size(67, 17);
            this.RadioMulticast.TabIndex = 14;
            this.RadioMulticast.TabStop = true;
            this.RadioMulticast.Text = "Multicast";
            this.RadioMulticast.UseVisualStyleBackColor = true;
            this.RadioMulticast.CheckedChanged += new System.EventHandler(this.RadioMulticast_CheckedChanged);
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.SetPlaybackTakeButton);
            this.tabPage2.Controls.Add(this.PlaybackTakeNameText);
            this.tabPage2.Controls.Add(this.StopRecordButton);
            this.tabPage2.Controls.Add(this.SetRecordingTakeButton);
            this.tabPage2.Controls.Add(this.RecordingTakeNameText);
            this.tabPage2.Controls.Add(this.TimelineStopButton);
            this.tabPage2.Controls.Add(this.LiveModeButton);
            this.tabPage2.Controls.Add(this.RecordButton);
            this.tabPage2.Controls.Add(this.EditModeButton);
            this.tabPage2.Controls.Add(this.TimelinePlayButton);
            this.tabPage2.Location = new System.Drawing.Point(4, 22);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(355, 160);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "Commands";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // SetPlaybackTakeButton
            // 
            this.SetPlaybackTakeButton.Location = new System.Drawing.Point(11, 39);
            this.SetPlaybackTakeButton.Name = "SetPlaybackTakeButton";
            this.SetPlaybackTakeButton.Size = new System.Drawing.Size(171, 23);
            this.SetPlaybackTakeButton.TabIndex = 26;
            this.SetPlaybackTakeButton.Text = "Set Playback Take Name";
            this.SetPlaybackTakeButton.UseVisualStyleBackColor = true;
            this.SetPlaybackTakeButton.Click += new System.EventHandler(this.SetPlaybackTakeButton_Click);
            // 
            // PlaybackTakeNameText
            // 
            this.PlaybackTakeNameText.Location = new System.Drawing.Point(188, 41);
            this.PlaybackTakeNameText.Name = "PlaybackTakeNameText";
            this.PlaybackTakeNameText.Size = new System.Drawing.Size(161, 20);
            this.PlaybackTakeNameText.TabIndex = 25;
            // 
            // StopRecordButton
            // 
            this.StopRecordButton.Location = new System.Drawing.Point(122, 69);
            this.StopRecordButton.Name = "StopRecordButton";
            this.StopRecordButton.Size = new System.Drawing.Size(99, 23);
            this.StopRecordButton.TabIndex = 24;
            this.StopRecordButton.Text = "Stop Record";
            this.StopRecordButton.UseVisualStyleBackColor = true;
            this.StopRecordButton.Click += new System.EventHandler(this.StopRecordButton_Click);
            // 
            // SetRecordingTakeButton
            // 
            this.SetRecordingTakeButton.Location = new System.Drawing.Point(11, 10);
            this.SetRecordingTakeButton.Name = "SetRecordingTakeButton";
            this.SetRecordingTakeButton.Size = new System.Drawing.Size(171, 23);
            this.SetRecordingTakeButton.TabIndex = 23;
            this.SetRecordingTakeButton.Text = "Set Recording Take Name";
            this.SetRecordingTakeButton.UseVisualStyleBackColor = true;
            this.SetRecordingTakeButton.Click += new System.EventHandler(this.SetRecordingTakeButton_Click);
            // 
            // RecordingTakeNameText
            // 
            this.RecordingTakeNameText.Location = new System.Drawing.Point(188, 12);
            this.RecordingTakeNameText.Name = "RecordingTakeNameText";
            this.RecordingTakeNameText.Size = new System.Drawing.Size(161, 20);
            this.RecordingTakeNameText.TabIndex = 22;
            // 
            // EditModeButton
            // 
            this.EditModeButton.Location = new System.Drawing.Point(122, 100);
            this.EditModeButton.Name = "EditModeButton";
            this.EditModeButton.Size = new System.Drawing.Size(99, 23);
            this.EditModeButton.TabIndex = 19;
            this.EditModeButton.Text = "Edit Mode";
            this.EditModeButton.UseVisualStyleBackColor = true;
            this.EditModeButton.Click += new System.EventHandler(this.EditModeButton_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.Gainsboro;
            this.ClientSize = new System.Drawing.Size(953, 664);
            this.Controls.Add(this.tabControl1);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.chart1);
            this.Controls.Add(this.listView1);
            this.Controls.Add(this.dataGridView1);
            this.Name = "Form1";
            this.Text = "NatNet Managed Client Sample";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart1)).EndInit();
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage1.PerformLayout();
            this.tabPage2.ResumeLayout(false);
            this.tabPage2.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.DataGridView dataGridView1;
        private System.Windows.Forms.ListView listView1;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.CheckBox checkBoxConnect;
        private System.Windows.Forms.ColumnHeader columnHeader2;
        private System.Windows.Forms.Button buttonGetDataDescriptions;
        private System.Windows.Forms.DataVisualization.Charting.Chart chart1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Timer UpdateUITimer;
        private System.Windows.Forms.Button RecordButton;
        private System.Windows.Forms.Button TimelineStopButton;
        private System.Windows.Forms.Button LiveModeButton;
        private System.Windows.Forms.Button TimelinePlayButton;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.Label Local;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox textBoxServer;
        private System.Windows.Forms.RadioButton RadioUnicast;
        private System.Windows.Forms.RadioButton RadioMulticast;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.Button SetRecordingTakeButton;
        private System.Windows.Forms.TextBox RecordingTakeNameText;
        private System.Windows.Forms.Button EditModeButton;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button StopRecordButton;
        private System.Windows.Forms.ComboBox comboBoxLocal;
        private System.Windows.Forms.DataGridViewTextBoxColumn ID;
        private System.Windows.Forms.DataGridViewTextBoxColumn X;
        private System.Windows.Forms.DataGridViewTextBoxColumn Y;
        private System.Windows.Forms.DataGridViewTextBoxColumn Z;
        private System.Windows.Forms.DataGridViewTextBoxColumn Yaw;
        private System.Windows.Forms.DataGridViewTextBoxColumn Pitch;
        private System.Windows.Forms.DataGridViewTextBoxColumn Roll;
        private System.Windows.Forms.Button SetPlaybackTakeButton;
        private System.Windows.Forms.TextBox PlaybackTakeNameText;
    }
}

