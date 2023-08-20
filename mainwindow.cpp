//-------------------------------------------------
//
// Project: NMEA Message Parser
// Created by: Burak Okur
// Creation Date: 2023-08-05
//
//-------------------------------------------------

// NMEA Message Parser Project
// This project provides a comprehensive parser for NMEA messages commonly used in GPS and navigation systems.
// It is designed to extract and organize relevant information from various NMEA sentence formats.
// Two primary message types, $GPRMC and $GPROT, are supported in this parser.
//
// Example NMEA sentences:
// $GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A
// $GPROT,35.6,A*4E
//
// Feel free to use and modify this code for your own projects. If you have any questions or suggestions,
// Please reach out to me at [burak1837burak@gmail.com].

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setvisible();            // Initialize UI visibility settings
    defaultsettings();       // Load default settings
    mydesign();              // Apply custom UI design

    COMPORT1 = new QSerialPort();   // Initialize first serial port
    COMPORT2 = new QSerialPort();   // Initialize second serial port

    Timer = new QTimer(this);       // Create a timer for periodic actions
    connect(Timer, SIGNAL(timeout()), this, SLOT(on_pushButton_start_clicked()));
    Timer->setInterval(1000);       // Set timer interval to 1000ms (1 second)

    connect(COMPORT2, SIGNAL(readyRead()), this, SLOT(Read_Data()));  // Connect serial port signal to data reading slot
    ui->groupBoxROT->setVisible(false); // Set ROT group box to initially be hidden
}

// Apply custom UI design settings
void MainWindow::mydesign()
{
    // Apply custom UI design settings
    ui->comboBox_choose_data->setFixedWidth(70);

    // Custom styling for buttons
    ui->pushButton_start->setStyleSheet(
        "QPushButton {"
        "   border: 2px outset #3498db;"
        "   border-radius: 8px;"
        "   padding: 2px 4px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #78b9ff;"
        "   border: 2px solid #0b57a9;"
        "}"
        "QPushButton:disabled {"
        "   background-color: #0b57a9;"
        "   border: 1px solid #3498db;"
        "}"
    );

    ui->pushButton_stop->setStyleSheet(
        "QPushButton {"
        "   border: 2px outset #3498db;"
        "   border-radius: 8px;"
        "   padding: 2px 4px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #78b9ff;"
        "   border: 2px solid #0b57a9;"
        "}"
        "QPushButton:disabled {"
        "   background-color: #0b57a9;"
        "   border: 1px solid #3498db;"
        "}"
    );

    ui->pushButton_Sensor_add->setStyleSheet(
        "QPushButton {"
        "   border: 3px outset #bababa;"
        "   border-radius: 4px;"
        "   padding: 6px 14px;"
        "   color: #333;"
        "   background-color: #f5f5f5;"
        "}"
        "QPushButton:hover {"
        "   background-color: #e0e0e0;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #d3d3d3;"
        "}"
    );
}

// Clear labels displaying data values
void MainWindow::sclear()
{
    // Clear labels displaying data values
    ui->label_a1->clear();
    ui->label_a2->clear();
    ui->label_a3->clear();
    ui->label_a4->clear();
    ui->label_a5->clear();
    ui->label_a6->clear();
    ui->label_a7->clear();
    ui->label_a8->clear();
    ui->label_a9->clear();
    ui->label_a10->clear();
    ui->label_a11->clear();

    ui->label_b1->clear();
    ui->label_b2->clear();
    ui->label_b3->clear();
    ui->label_b4->clear();
    ui->label_b5->clear();
    ui->label_b6->clear();
    ui->label_b7->clear();
    ui->label_b8->clear();
    ui->label_b9->clear();
    ui->label_b10->clear();
    ui->label_b11->clear();

    ui->label_sat->clear();
}

// Set visibility of UI elements as needed
void MainWindow::setvisible()
{
    // Hide labels displaying data values
    ui->label_x1->setVisible(false);
    ui->label_x2->setVisible(false);
    ui->label_x3->setVisible(false);
    ui->label_x4->setVisible(false);
    ui->label_x5->setVisible(false);
    ui->label_x6->setVisible(false);
    ui->label_x7->setVisible(false);
    ui->label_x8->setVisible(false);
    ui->label_x9->setVisible(false);
    ui->label_x10->setVisible(false);
}

// Test if characters in a string are valid in teststring
QString MainWindow::satellite(QChar data2)
{
    // Map satellite system code to its name
    // Return the corresponding name based on the code received
    // Default to "INVALID" if no match is found

    if (data2 == 'A')
    {
        return "GA European Global Navigation System (Galileo)";
    }
    else if (data2 == 'B')
    {
        return "GB BeiDou Navigation Satellite System (BDS)";
    }
    else if (data2 == 'I')
    {
        return "Navigation Indian Constellation (NavIC)";
    }
    else if (data2 == 'L')
    {
        return "GL Globalnaya Navigazionnaya Sputnikovaya Sistema (GLONASS)";
    }
    else if (data2 == 'N')
    {
        return "GN Global Navigation Satellite System (GNSS)";
    }
    else if (data2 == 'P')
    {
        return "GP Global Positioning System (GPS)";
    }
    else if (data2 == 'Q')
    {
        return "GQ Quasi-Zenith Satellite System (QZSS)";
    }
    else
    {
        return "INVALID";
    }
}

bool MainWindow::lettertest(QString data3)
{
    // Create a list to test the data
    QString lattest[11];
    bool test; // Flag to indicate the validity of the characters

    // Populate the list with numeric characters and decimal point
    for (int i = 0; i < 10; i++)
    {
        lattest[i] = QString::number(i);
    }
    lattest[10] = '.';

    // Test each character in the input string
    for (int j = 0; j < data3.length(); j++)
    {
        test = false;
        for (int i = 0; i < 11; i++)
        {
            if (data3[j] == lattest[i])
            {
                test = true; // Set flag to true if a valid character is found
                break;
            }
        }
        if (test == false)
        {
            break;
        }
    }
    // Return the overall validity of the characters in the input string
    return test;
}

// Convert an integer to its hexadecimal representation
string MainWindow::inttohex(int number)
{
    // Convert the integer to its hexadecimal representation
    std::stringstream stream;
    stream << std::hex << std::uppercase << number;
    return stream.str(); // Return the hexadecimal representation as a string
}

// Calculate the checksum for a given NMEA data string
int MainWindow::calculate_csum(string data1)
{
    // Extract data part from the NMEA string
    string data = data1.substr(data1.find("$") + 1, data1.find("*") - 1);
    int checksum = 0;
    // Calculate XOR checksum of all characters in the data part
    for (int i = 0; i < data.size(); i++)
    {
        checksum ^= int(data[i]);
    }
    // Return the calculated checksum value
    return checksum;
}

// Parse function for data in lineEdit_INPUT
void MainWindow::nmeaparser(string data) {
    // NMEA Sentence Parsing
    // This section contains functions to parse various types of NMEA sentences.
    string datac = data; //Store a copy of the original data for checksum calculation

    // Split the NMEA sentence into segments using ',' as the delimiter
    QString nmeapars[15];  // Array to store parsed segments
    string delimiter = ",";
    size_t pos = 0;
    QString token;
    int j = 0;

    while ((pos = data.find(delimiter)) != string::npos) {
        QString vrb = QString::fromStdString(data.substr(0, pos));
        token = vrb;
        nmeapars[j] = token;
        j++;
        data.erase(0, pos + delimiter.length());
    }
    nmeapars[j] = QString::fromStdString(data); // Convert remaining data to QString

    //Variables for message check
    // Time Check
    // Extract hours, minutes, and seconds from the UTC time in the NMEA sentence
    int hours = (nmeapars[1].mid(0, 2)).toInt();
    int minutes = (nmeapars[1].mid(2, 2)).toInt();
    int seconds = (nmeapars[1].mid(4, 2)).toInt();

    // Date Check
    // Extract day, month, and year from the date in the NMEA sentence
    int days = (nmeapars[9].mid(0, 2)).toInt();
    int months = (nmeapars[9].mid(2, 2)).toInt();
    int years = (nmeapars[9].mid(4, 2)).toInt();

    // CHECKSUM
    // Calculate the checksum of the NMEA sentence and convert it to hexadecimal
    int checksum = calculate_csum(datac); // Call a function to calculate the checksum
    string csumh = inttohex(checksum);     // Convert the checksum to hexadecimal representation

    // Get satellite identifier
    int statval = 0;
    // Extract satellite identifier from the NMEA sentence
    QChar sat = nmeapars[0][2]; // $G(P)ROT

    // Determine if the sentence is ROT or RMC type
    QString rotrmc = nmeapars[0];
    QString rotrmc_subString = rotrmc.mid(3, 3); // $GP(RMC/ROT)

    if (rotrmc_subString == "RMC") {
        // Const labels
        ui->label_a1->setText("Message ID:");
        ui->label_a2->setText("UTC:");
        ui->label_a3->setText("Status:");
        ui->label_a4->setText("Latitude:");
        ui->label_a5->setText("Longitude:");
        ui->label_a6->setText("Speed:");
        ui->label_a7->setText("Track angle:");
        ui->label_a8->setText("Date:");
        ui->label_a9->setText("Magnetic variation:");
        ui->label_a10->setText("Checksum:");
        ui->label_a11->setText("Checksum?:");

        // Variable labels
        ui->label_b1->setText(nmeapars[0]);
        ui->label_b2->setText(nmeapars[1].mid(0, 2) + ':' + nmeapars[1].mid(2, 2) + ':' + nmeapars[1].mid(4, 2));
        ui->label_b3->setText(nmeapars[2]);
        ui->label_b4->setText(nmeapars[3] + " " + nmeapars[4]);
        ui->label_b5->setText(nmeapars[5] + " " + nmeapars[6]);
        ui->label_b6->setText(nmeapars[7]);
        ui->label_b7->setText(nmeapars[8]);
        ui->label_b8->setText(nmeapars[9].mid(0, 2) + '.' + nmeapars[9].mid(2, 2) + '.' + nmeapars[9].mid(4, 2));
        ui->label_b9->setText(nmeapars[10] + " " + nmeapars[11].mid(0, 1));
        ui->label_b10->setText(nmeapars[11].mid(1, 3)); // Checksum ayirma
        ui->label_b11->setText("*" + QString::fromStdString(csumh));
        ui->label_sat->setText(satellite(sat));
    } else if (rotrmc_subString == "ROT") {
        // Const labels
        ui->label_a1->setText("Message ID:");
        ui->label_a2->setText("Rate of turn:");
        ui->label_a3->setText("Status:");
        ui->label_a4->setText("Checksum:");
        ui->label_a5->setText("Checksum?:");
        ui->label_a6->clear(); // ROT does not have more data
        ui->label_a7->clear();
        ui->label_a8->clear();
        ui->label_a9->clear();
        ui->label_a10->clear();
        ui->label_a11->clear();

        // Variable labels
        ui->label_b1->setText(nmeapars[0]);
        ui->label_b2->setText(nmeapars[1]);
        ui->label_b3->setText(nmeapars[2].mid(0, 1)); // A
        ui->label_b4->setText(nmeapars[2].mid(1, 3)); // *4E checksum ayirma
        ui->label_b5->setText("*" + QString::fromStdString(csumh));
        ui->label_b6->clear(); // ROT does not have more data
        ui->label_b7->clear();
        ui->label_b8->clear();
        ui->label_b9->clear();
        ui->label_b10->clear();
        ui->label_b11->clear();
        ui->label_sat->setText(satellite(sat));
    } else {
        // Clear labels and display "Message ID:" if sentence type is unknown
        sclear();
        ui->label_a1->setText("Message ID:");
    }

    // Default settings
    setvisible();
    // ------------------------- GPRMC CHECK --------------------------------------
    // This section checks the validity of the GPRMC NMEA sentence and updates the GUI accordingly.
    if (rotrmc_subString == "RMC") {
        QString cdata = nmeapars[11]; // A*4E // for checksum

        // ************************** $GPRCM **********************************
        // Check if the NMEA sentence starts with "$"
        if (nmeapars[0][0] != '$') { // ($)GPROT
            ui->label_b1->setText("INVALID DATA");
            ui->label_x1->setVisible(true);
            statval++;
        }

        // Check if the second character is 'G'
        if (nmeapars[0][1] != 'G') { // $(G)PROT
            ui->label_b1->setText("INVALID DATA");
            ui->label_x1->setVisible(true);
            statval++;
        }
        // Check satellite validity and sentence type
        else if (satellite(sat) == "INVALID") { // $G(P)ROT
            ui->label_b1->setText("INVALID DATA");
            ui->label_sat->setText("INVALID DATA");
            ui->label_x1->setVisible(true);
            statval++;
        }
        else if (rotrmc_subString != "RMC") { // $GP(RMC)
            ui->label_b1->setText("INVALID DATA");
            ui->label_x1->setVisible(true);
            statval++;
        }
        // Check sentence length
        else if (nmeapars[0].length() != 6) {
            ui->label_b1->setText("INVALID DATA");
            ui->label_x1->setVisible(true);
            statval++;
        }

        // ************************** UTC ************************************
        // Check if the hours, minutes, and seconds values are valid
        if ((hours < 0 || hours > 23) || (minutes < 0 || minutes > 59) || (seconds < 0 || seconds > 59)) {
            ui->label_b2->setText("INVALID DATA");
            statval++;
            ui->label_x2->setVisible(true);
        }
        // Check if the UTC time value has the correct length
        else if (nmeapars[1].length() != 6) {
            ui->label_b2->setText("INVALID DATA");
            ui->label_x2->setVisible(true);
            statval++;
        }
        // Check if the UTC time value contains valid characters
        else if (lettertest(nmeapars[1]) == false) {
            ui->label_b2->setText("INVALID DATA");
            ui->label_x2->setVisible(true);
            statval++;
        }

        // ************************** Status *********************************
        // Check if the status value is either 'A' (active) or 'V' (void)
        if ((nmeapars[2] != 'A') && (nmeapars[2] != 'V')) {
            ui->label_b3->setText("INVALID DATA");
            ui->label_x3->setVisible(true);
            statval++;
        }

        // ************************** Latitude *******************************
        // Check if the latitude value contains valid characters
        if (lettertest(nmeapars[3]) == false) {
            ui->label_b4->setText("INVALID DATA");
            ui->label_x4->setVisible(true);
            statval++;
        }
        // Check if the latitude direction is either 'N' or 'S'
        else if ((nmeapars[4] != 'N') && (nmeapars[4] != 'S')) { // NORTH SOUTH
            ui->label_b4->setText("INVALID DATA");
            ui->label_x4->setVisible(true);
            statval++;
        }

        // ************************** Longitude ******************************
        // Check if the longitude value contains valid characters
        if (lettertest(nmeapars[5]) == false) {
            ui->label_b5->setText("INVALID DATA");
            ui->label_x5->setVisible(true);
            statval++;
        }
        // Check if the longitude direction is either 'W' or 'E'
        else if ((nmeapars[6] != 'W') && (nmeapars[6] != 'E')) { // EAST WEST
            ui->label_b5->setText("INVALID DATA");
            ui->label_x5->setVisible(true);
            statval++;
        }

        // ************************** Speed **********************************
        // Check if the speed value is a valid value
        if (lettertest(nmeapars[7]) == false) {
            ui->label_b6->setText("INVALID DATA");
            ui->label_x6->setVisible(true);
            statval++;
        }

        // ************************** Track angle ****************************
        // Check if the track angle value is a valid value
        if (lettertest(nmeapars[8]) == false) {
            ui->label_b7->setText("INVALID DATA");
            ui->label_x7->setVisible(true);
            statval++;
        }

        // ************************** Date ***********************************
        // Check if the day, month, and year values are valid
        if ((days < 0 || days > 31) || (months < 0 || months > 12) || (years < 0 || years > 99)) {
            ui->label_b8->setText("INVALID DATA");
            ui->label_x8->setVisible(true);
            statval++;
        }
        // Check if the date value contains valid characters
        else if (lettertest(nmeapars[9]) == false) {
            ui->label_b8->setText("INVALID DATA");
            ui->label_x8->setVisible(true);
            statval++;
        }
        // Check if the date value has the correct length
        else if (nmeapars[9].length() != 6) {
            ui->label_b8->setText("INVALID DATA");
            ui->label_x8->setVisible(true);
            statval++;
        }

        // ************************** Magnetic variation **********************
        // Check if the magnetic variation is a valid value
        if (lettertest(nmeapars[10]) == false) {
            ui->label_b9->setText("INVALID DATA");
            ui->label_x9->setVisible(true);
            statval++;
        }
        // Check if the direction indicator (E/W/N/S) is valid
        else if ((nmeapars[11][0] != 'W') && (nmeapars[11][0] != 'E') && (nmeapars[11][0] != 'N') && (nmeapars[11][0] != 'S')) { // EAST WEST
            ui->label_b9->setText("INVALID DATA");
            ui->label_x9->setVisible(true);
            statval++;
        }

        // ************************** Checksum *********************************
        // Check if the checksum data is formatted correctly
        if (cdata.mid(1, 3)[0] != '*') {
            ui->label_b10->setText("INVALID DATA");
            ui->label_x10->setVisible(true);
            statval++;
        }
        // Check if the calculated checksum matches the provided checksum
        else if (csumh != cdata.mid(2, 2).toStdString()) {
            ui->label_b10->setText("Checksum error!");
            ui->label_x10->setVisible(true);
            statval++;
        }
        // Check if the checksum data length is valid
        else if (nmeapars[11].length() > 4) {
            ui->label_b10->setText("INVALID DATA");
            ui->label_x10->setVisible(true);
            statval++;
        }

        // ************************** Message size ***************************
        // Check if the number of parsed fields is correct
        if (j != 11) {
            ui->label_status->setText("INVALID DATA");
            statval++;
            sclear();
            setvisible();
        }
    }
    // ------------------------- GPROT CHECK -------------------------------
    // This section checks the validity of the GPROT NMEA sentence and updates the GUI accordingly.

    else if (rotrmc_subString == "ROT") {
        QString vdata = nmeapars[2]; // Extract checksum data for validation

        // ************************** $GPROT **********************************
        // Check if the NMEA sentence starts with "$"
        if (nmeapars[0][0] != '$') { // ($)GPROT
            statval++;
            ui->label_b1->setText("INVALID DATA");
            ui->label_x1->setVisible(true);
        }
        // Check if the second character is 'G'
        else if (nmeapars[0][1] != 'G') { // $(G)PROT
            statval++;
            ui->label_b1->setText("INVALID DATA");
            ui->label_x1->setVisible(true);
        }
        // Check satellite validity and sentence type
        else if (satellite(sat) == "INVALID") { // $G(P)ROT
            statval++;
            ui->label_sat->setText("INVALID DATA");
            ui->label_b1->setText("INVALID DATA");
            ui->label_x1->setVisible(true);
        } else if (rotrmc_subString != "ROT") { // $GP(RMC)
            statval++;
            ui->label_b1->setText("INVALID DATA");
            ui->label_x1->setVisible(true);
        }
        // Check sentence length
        else if (nmeapars[0].length() != 6) {
            statval++;
            ui->label_b1->setText("INVALID DATA");
            ui->label_x1->setVisible(true);
        }

        // ************************** Rate of turn *****************************
        // Check if the rate of turn value is a valid value
        if (lettertest(nmeapars[1]) == false) {
            statval++;
            ui->label_b2->setText("INVALID DATA");
            ui->label_x2->setVisible(true);
        } else {
            // Clear any previous status messages
            ui->label_status->clear();
        }

        // ************************** Status *********************************
        // Check if the status value is either 'A' (active) or 'V' (void)
        if ((vdata.mid(0, 1) != 'A') && (vdata.mid(0, 1) != 'V')) {
            statval++;
            ui->label_b3->setText("INVALID DATA");
            ui->label_x3->setVisible(true);
        }

        // ************************** Checksum *********************************
        // Check if the checksum data starts with "*"
        if (vdata.mid(1, 3)[0] != '*') {
            ui->label_b4->setText("INVALID DATA");
            ui->label_x4->setVisible(true);
            statval++;
        }
        // Compare calculated checksum with received checksum
        else if (csumh != vdata.mid(2, 2).toStdString()) {
            ui->label_b4->setText("Checksum error!");
            ui->label_x4->setVisible(true);
            statval++;
        }
        // Check if the checksum value has the correct length
        else if (nmeapars[2].length() > 4) {
            ui->label_b4->setText("INVALID DATA");
            ui->label_x4->setVisible(true);
            statval++;
        }

        // ************************** Message size ***************************
        // Check if the number of data values parsed is correct for the ROT sentence
        if (j != 2) {
            ui->label_status->setText("INVALID DATA");
            statval++;
            sclear();
            setvisible();
        }
    }
    // If none of the valid cases match, mark the sentence as INVALID DATA
    else {
        statval++;
        ui->label_b1->setText("INVALID DATA");
        ui->label_x1->setVisible(true);
    }

    // Check overall error status and display appropriate message
    // If any validation errors occurred, set the overall status message as INVALID DATA
    if (statval > 0) {
        ui->label_status->setText("INVALID DATA");
    }
    // If no validation errors occurred, clear the overall status message
    else if (statval == 0) {
        ui->label_status->clear();
    }

}

// Save application settings to an INI file
void MainWindow::savesettings()
{
    // Create a QSettings object to manage application settings
    QSettings settings("MySettings.ini", QSettings::Format::IniFormat);

    // Save PORTSET settings
    settings.setValue("PORTSET/comboBox_com1", ui->comboBox_com1->currentIndex());
    settings.setValue("PORTSET/comboBox_com2", ui->comboBox_com2->currentIndex());
    settings.setValue("PORTSET/comboBox_baudrate", ui->comboBox_baudrate->currentIndex());
    settings.setValue("PORTSET/comboBox_parity", ui->comboBox_parity->currentIndex());
    settings.setValue("PORTSET/comboBox_databits", ui->comboBox_databits->currentIndex());
    settings.setValue("PORTSET/comboBox_stopbits", ui->comboBox_stopbits->currentIndex());
    settings.setValue("PORTSET/comboBox_flowcontrol", ui->comboBox_flowcontrol->currentIndex());

    // Save SENSORSET settings for RMC group
    settings.beginGroup("RMC");
    settings.setValue("spinBox_hours", ui->spinBox_hours->value());
    settings.setValue("spinBox_minutes", ui->spinBox_minutes->value());
    settings.setValue("spinBox_seconds", ui->spinBox_seconds->value());
    settings.setValue("spinBox_days", ui->spinBox_days->value());
    settings.setValue("spinBox_months", ui->spinBox_months->value());
    settings.setValue("spinBox_years", ui->spinBox_years->value());

    settings.setValue("doubleSpinBox_2", ui->doubleSpinBox_2->value());
    settings.setValue("doubleSpinBox_3", ui->doubleSpinBox_3->value());
    settings.setValue("doubleSpinBox_4", ui->doubleSpinBox_4->value());
    settings.setValue("doubleSpinBox_5", ui->doubleSpinBox_5->value());
    settings.setValue("doubleSpinBox_8", ui->doubleSpinBox_8->value());

    settings.setValue("comboBox_csat", ui->comboBox_csat->currentText());
    settings.setValue("comboBox_statusrmc", ui->comboBox_statusrmc->currentText());
    settings.setValue("comboBox_15", ui->comboBox_15->currentText());
    settings.setValue("comboBox_16", ui->comboBox_16->currentText());
    settings.setValue("comboBox_17", ui->comboBox_17->currentText());
    settings.endGroup();

    // Save SENSORSET settings for ROT group
    settings.beginGroup("ROT");
    settings.setValue("comboBox_rsat", ui->comboBox_rsat->currentText());
    settings.setValue("comboBox_statusrot", ui->comboBox_statusrot->currentText());
    settings.setValue("doubleSpinBox_roturns", ui->doubleSpinBox_roturns->value());
    settings.endGroup();
}

// Load and apply default settings from the INI file
void MainWindow::defaultsettings()
{
    // Create a QSettings object to access application settings
    QSettings settings("MySettings.ini", QSettings::Format::IniFormat);

    // Load and set PORTSET settings
    ui->comboBox_com1->setCurrentIndex(settings.value("PORTSET/comboBox_com1", 0).toInt());
    ui->comboBox_com2->setCurrentIndex(settings.value("PORTSET/comboBox_com2", 0).toInt());
    ui->comboBox_baudrate->setCurrentIndex(settings.value("PORTSET/comboBox_baudrate", 0).toInt());
    ui->comboBox_parity->setCurrentIndex(settings.value("PORTSET/comboBox_parity", 0).toInt());
    ui->comboBox_databits->setCurrentIndex(settings.value("PORTSET/comboBox_databits", 0).toInt());
    ui->comboBox_stopbits->setCurrentIndex(settings.value("PORTSET/comboBox_stopbits", 0).toInt());
    ui->comboBox_flowcontrol->setCurrentIndex(settings.value("PORTSET/comboBox_flowcontrol", 0).toInt());

    // Load and set SENSORSET settings for RMC group
    settings.beginGroup("RMC");
    ui->spinBox_hours->setValue(settings.value("spinBox_hours", 0).toInt());
    ui->spinBox_minutes->setValue(settings.value("spinBox_minutes", 0).toInt());
    ui->spinBox_seconds->setValue(settings.value("spinBox_seconds", 0).toInt());
    ui->spinBox_days->setValue(settings.value("spinBox_days", 0).toInt());
    ui->spinBox_months->setValue(settings.value("spinBox_months", 0).toInt());
    ui->spinBox_years->setValue(settings.value("spinBox_years", 0).toInt());

    ui->doubleSpinBox_2->setValue(settings.value("doubleSpinBox_2", 0.0).toDouble());
    ui->doubleSpinBox_3->setValue(settings.value("doubleSpinBox_3", 0.0).toDouble());
    ui->doubleSpinBox_4->setValue(settings.value("doubleSpinBox_4", 0.0).toDouble());
    ui->doubleSpinBox_5->setValue(settings.value("doubleSpinBox_5", 0.0).toDouble());
    ui->doubleSpinBox_8->setValue(settings.value("doubleSpinBox_8", 0.0).toDouble());

    ui->comboBox_csat->setCurrentText(settings.value("comboBox_csat", "").toString());
    ui->comboBox_statusrmc->setCurrentText(settings.value("comboBox_statusrmc", "").toString());
    ui->comboBox_15->setCurrentText(settings.value("comboBox_15", "").toString());
    ui->comboBox_16->setCurrentText(settings.value("comboBox_16", "").toString());
    ui->comboBox_17->setCurrentText(settings.value("comboBox_17", "").toString());
    settings.endGroup();

    // Load and set SENSORSET settings for ROT group
    settings.beginGroup("ROT");
    ui->comboBox_rsat->setCurrentText(settings.value("comboBox_rsat", "").toString());
    ui->comboBox_statusrot->setCurrentText(settings.value("comboBox_statusrot", "").toString());
    ui->doubleSpinBox_roturns->setValue(settings.value("doubleSpinBox_roturns", 0.0).toDouble());
    settings.endGroup();
}

// Configure serial port settings based on user-selected options
void MainWindow::portsettings()
{
    // Close both COM ports
    COMPORT1->close();
    COMPORT2->close();

    // Set port names for COMPORT1 and COMPORT2 based on current comboBox selections
    COMPORT1->setPortName(ui->comboBox_com1->currentText());
    COMPORT2->setPortName(ui->comboBox_com2->currentText());

    // Set baud rate for both COM ports
    QString selectedBaudRateStr = ui->comboBox_baudrate->currentText();
    int selectedBaudRateInt = selectedBaudRateStr.toInt();
    QSerialPort::BaudRate selectedBaudRate = static_cast<QSerialPort::BaudRate>(selectedBaudRateInt);
    COMPORT1->setBaudRate(selectedBaudRate);
    COMPORT2->setBaudRate(selectedBaudRate);

    // Set parity for both COM ports
    QString selectedParityStr = ui->comboBox_parity->currentText();
    QSerialPort::Parity selectedParity = QSerialPort::NoParity; // Default value

    if (selectedParityStr == "Even")
        selectedParity = QSerialPort::EvenParity;
    else if (selectedParityStr == "Odd")
        selectedParity = QSerialPort::OddParity;
    else if (selectedParityStr == "Mark")
        selectedParity = QSerialPort::MarkParity;
    else if (selectedParityStr == "Space")
        selectedParity = QSerialPort::SpaceParity;

    // Set data bits for both COM ports
    QString dataBitsText = ui->comboBox_databits->currentText();
    QSerialPort::DataBits selectedDataBits = QSerialPort::Data8; // Default value

    if (dataBitsText == "5")
        selectedDataBits = QSerialPort::Data5;
    else if (dataBitsText == "6")
        selectedDataBits = QSerialPort::Data6;
    else if (dataBitsText == "7")
        selectedDataBits = QSerialPort::Data7;

    // Set stop bits for both COM ports
    QString stopBitsText = ui->comboBox_stopbits->currentText();
    QSerialPort::StopBits selectedStopBits = QSerialPort::OneStop; // Default value

    if (stopBitsText == "OneAndHalf")
        selectedStopBits = QSerialPort::OneAndHalfStop;
    else if (stopBitsText == "Two")
        selectedStopBits = QSerialPort::TwoStop;

    // Set flow control for both COM ports
    QString flowControlText = ui->comboBox_flowcontrol->currentText();
    QSerialPort::FlowControl selectedFlowControl = QSerialPort::NoFlowControl; // Default value

    if (flowControlText == "Hardware")
        selectedFlowControl = QSerialPort::HardwareControl;
    else if (flowControlText == "Software")
        selectedFlowControl = QSerialPort::SoftwareControl;

    // Set data bits, stop bits, and flow control for both COM ports
    COMPORT1->setDataBits(selectedDataBits);
    COMPORT2->setDataBits(selectedDataBits);
    COMPORT1->setStopBits(selectedStopBits);
    COMPORT2->setStopBits(selectedStopBits);
    COMPORT1->setFlowControl(selectedFlowControl);
    COMPORT2->setFlowControl(selectedFlowControl);

    // Open both COM ports
    COMPORT1->open(QIODevice::ReadWrite);
    COMPORT2->open(QIODevice::ReadWrite);

    // Update UI labels based on COM port status
    ui->label_comport1->setText(COMPORT1->isOpen() ? "OPEN" : "CLOSE");
    ui->label_comport2->setText(COMPORT2->isOpen() ? "OPEN" : "CLOSE");

    // Save the current settings of the comboboxes
    savesettings();
}


MainWindow::~MainWindow()
{
    delete ui;
}

// Handle the click event of the "Start" button
void MainWindow::on_pushButton_start_clicked()
{
    // Update RTC and RTD data based on checkbox states
    on_checkBox_rtc_stateChanged();
    on_checkBox_rtd_stateChanged();

    // Disable the "Start" button and enable the "Stop" button
    ui->pushButton_start->setEnabled(false);
    ui->pushButton_stop->setEnabled(true);

    // Clear displayed data labels and set visibility
    sclear();
    setvisible();

    // Apply port settings
    portsettings();

    // Get input data from the text line edit
    QString datasend = ui->lineEdit_INPUT->text();
    QByteArray data = datasend.toUtf8();

    // Send data to COMPORT1 if it's not empty
    if (!data.isEmpty())
        COMPORT1->write(data);
    else
    {
        // Clear labels and combo box if data is empty
        sclear();
        ui->comboBox_choose_data->clear();
    }

    // Start the timer for periodic actions
    Timer->start();
}

// Handle the click event of the "Stop" button
void MainWindow::on_pushButton_stop_clicked()
{
    // Enable the "Start" button and disable the "Stop" button
    ui->pushButton_start->setEnabled(true);
    ui->pushButton_stop->setEnabled(false);

    // Stop the timer
    Timer->stop();
}

// Handle incoming data from the serial port
void MainWindow::Read_Data() {

    // Read available data from COMPORT2
    QByteArray data9 = COMPORT2->readAll();

    if (!data9.isEmpty()) {
        QString datast = QString::fromUtf8(data9);
        string datac = datast.toStdString();

        // Check if the received data is a valid NMEA sentence
        if (datac[0] != '$' || (datac.substr(3, 3) != "RMC" && datac.substr(3, 3) != "ROT")) {
            // Update UI elements to indicate invalid data
            ui->label_a1->setText("Message ID:");
            ui->label_b1->setText("INVALID DATA");
            ui->label_x1->setVisible(true);
            ui->label_status->setText("INVALID DATA");
        } else if (datac.empty()) {
            // Update UI status message for empty data
            ui->label_status->setText("Enter Data");
        } else if (datac.size() < 5) {
            // Update UI status message for short data
            ui->label_status->setText("INVALID DATA");
        } else {
            // Clear the invalid data indicators
            ui->label_x1->setVisible(false);

            // Set the delimiter to identify parts of the data
            string delimiter = "<CR><LF>";
            size_t delimiterpos = datac.find(delimiter);

            // If delimiter is missing, add it to the end of the data
            if (delimiterpos == string::npos) {
                datac = datac + "<CR><LF>";
            }

            // Parse the received data into individual data parts
            vector<string> datapars;
            size_t delimiterpos1 = datac.find(delimiter);
            while (delimiterpos1 != string::npos) {
                string part = datac.substr(0, delimiterpos1);
                datapars.push_back(part);
                datac.erase(0, delimiterpos1 + delimiter.length());
                delimiterpos1 = datac.find(delimiter);
            }
            datapars.push_back(datac);

            // Update the combo box with parsed data count
            int comboboxsize = ui->comboBox_choose_data->count();
            if (comboboxsize != datapars.size()) {
                ui->comboBox_choose_data->clear();
                for (int i = 0; i < datapars.size(); i++) {
                    ui->comboBox_choose_data->addItem(QString::number(i + 1));
                }
                ui->comboBox_choose_data->setSizeAdjustPolicy(QComboBox::AdjustToContents);
                ui->comboBox_choose_data->setCurrentText(QString::number(comboboxsize + 1));
            }

            // Call nmeaparser function for selected data
            if (!datapars[1].empty()) {
                ui->comboBox_choose_data->setEnabled(true);
                for (int i = 0; i < datapars.size(); i++) {
                    if (ui->comboBox_choose_data->currentText() == QString::number(i + 1)) {
                        nmeaparser(datapars[i]); // Assuming nmeaparser function is defined elsewhere
                    }
                }
            } else {
                ui->comboBox_choose_data->setEnabled(false);
                ui->comboBox_choose_data->setCurrentText("1");
                nmeaparser(datapars[0]); // Assuming nmeaparser function is defined elsewhere
            }
        }
    } else {
        // Clear combo box and data
        ui->comboBox_choose_data->clear();
        data9.clear();
    }
}

// Handle the selection change in the combo box for message types
void MainWindow::on_comboBox_choose_id_currentIndexChanged(const QString &arg1) {

    // Check the selected message type
    if (arg1 == "RMC") {
        // If "RMC" is selected, show the "RMC" group box and hide the "ROT" group box
        ui->groupBoxRMC->setVisible(true);
        ui->groupBoxROT->setVisible(false);
    } else if (arg1 == "ROT") {
        // If "ROT" is selected, show the "ROT" group box and hide the "RMC" group box
        ui->groupBoxRMC->setVisible(false);
        ui->groupBoxROT->setVisible(true);
    }
}

// Handle the click event of the "Add Sensor" button
void MainWindow::on_pushButton_Sensor_add_clicked()
{
    // MYDINAMICARRAY
    // Append values from spin boxes to the dynamic array "spinBoxes"
    spinBoxes.append(QString::number(ui->spinBox_hours->value()));
    spinBoxes.append(QString::number(ui->spinBox_minutes->value()));
    spinBoxes.append(QString::number(ui->spinBox_seconds->value()));
    spinBoxes.append(QString::number(ui->spinBox_days->value()));
    spinBoxes.append(QString::number(ui->spinBox_months->value()));
    spinBoxes.append(QString::number(ui->spinBox_years->value()));

    // Ensure values in "spinBoxes" are two digits long
    for (int i = 0; i < 6; i++) {
        if (spinBoxes[i].length() < 2) {
            spinBoxes[i] = "0" + spinBoxes[i];
        }
    }

    QString datast = ui->lineEdit_INPUT->text();

    if (datast.length() > 0) {
        datast = datast + "<CR><LF>";

        QString nmeapars;
        QString message_id = ui->comboBox_choose_id->currentText();

        if (message_id == "RMC") {
            // Build NMEA sentence for "RMC" message type
            nmeapars = "$" + ui->comboBox_csat->currentText() + "RMC" +
                       ',' + spinBoxes[0] + spinBoxes[1] + spinBoxes[2] +
                       ',' + ui->comboBox_statusrmc->currentText() +
                       ',' + QString::number(ui->doubleSpinBox_2->value()) +
                       ',' + ui->comboBox_15->currentText() +
                       ',' + QString::number(ui->doubleSpinBox_5->value()) +
                       ',' + ui->comboBox_16->currentText() +
                       ',' + QString::number(ui->doubleSpinBox_4->value()) +
                       ',' + QString::number(ui->doubleSpinBox_3->value()) +
                       ',' + spinBoxes[3] + spinBoxes[4] + spinBoxes[5] +
                       ',' + QString::number(ui->doubleSpinBox_8->value()) +
                       ',' + ui->comboBox_17->currentText() + '*';
        } else if (message_id == "ROT") {
            // Build NMEA sentence for "ROT" message type
            nmeapars = "$" + ui->comboBox_rsat->currentText() + "ROT" +
                       ',' + QString::number(ui->doubleSpinBox_roturns->value()) +
                       ',' + ui->comboBox_statusrot->currentText() + '*';
        }

        spinBoxes.clear(); // ARRAY CLEARED
        int checksum = calculate_csum(nmeapars.toStdString());
        string csumh = inttohex(checksum);
        nmeapars += QString::fromStdString(csumh);
        datast += nmeapars;
        ui->lineEdit_INPUT->setText(datast);
    } else {
        QString nmeapars;
        QString message_id = ui->comboBox_choose_id->currentText();

        if (message_id == "RMC") {
            // Build NMEA sentence for "RMC" message type
            nmeapars = "$" + ui->comboBox_csat->currentText() + "RMC" +
                       ',' + spinBoxes[0] + spinBoxes[1] + spinBoxes[2] +
                       ',' + ui->comboBox_statusrmc->currentText() +
                       ',' + QString::number(ui->doubleSpinBox_2->value()) +
                       ',' + ui->comboBox_15->currentText() +
                       ',' + QString::number(ui->doubleSpinBox_5->value()) +
                       ',' + ui->comboBox_16->currentText() +
                       ',' + QString::number(ui->doubleSpinBox_4->value()) +
                       ',' + QString::number(ui->doubleSpinBox_3->value()) +
                       ',' + spinBoxes[3] + spinBoxes[4] + spinBoxes[5] +
                       ',' + QString::number(ui->doubleSpinBox_8->value()) +
                       ',' + ui->comboBox_17->currentText() + '*';
        } else if (message_id == "ROT") {
            // Build NMEA sentence for "ROT" message type
            nmeapars = "$" + ui->comboBox_rsat->currentText() + "ROT" +
                       ',' + QString::number(ui->doubleSpinBox_roturns->value()) +
                       ',' + ui->comboBox_statusrot->currentText() + '*';
        }

        spinBoxes.clear(); // ARRAY CLEARED
        int checksum = calculate_csum(nmeapars.toStdString());
        string csumh = inttohex(checksum);
        nmeapars += QString::fromStdString(csumh);
        ui->lineEdit_INPUT->setText(nmeapars);
    }
}

// Handle the state change of the RTC checkbox
void MainWindow::on_checkBox_rtc_stateChanged() {
    if (ui->checkBox_rtc->isChecked()) {
        // If RTC checkbox is checked, update spin boxes with current time
        QDateTime currentTime = QDateTime::currentDateTime();
        ui->spinBox_hours->setValue(currentTime.time().hour());
        ui->spinBox_minutes->setValue(currentTime.time().minute());
        ui->spinBox_seconds->setValue(currentTime.time().second());

        // Disable spin boxes for hours, minutes, and seconds
        ui->spinBox_hours->setEnabled(false);
        ui->spinBox_minutes->setEnabled(false);
        ui->spinBox_seconds->setEnabled(false);
    } else {
        // If RTC checkbox is unchecked, enable spin boxes for hours, minutes, and seconds
        ui->spinBox_hours->setEnabled(true);
        ui->spinBox_minutes->setEnabled(true);
        ui->spinBox_seconds->setEnabled(true);
    }
}

// Handle the state change of the RTD checkbox
void MainWindow::on_checkBox_rtd_stateChanged() {
    if (ui->checkBox_rtd->isChecked()) {
        // If RTD checkbox is checked, update spin boxes with current date
        QDate currentDate = QDate::currentDate();
        ui->spinBox_days->setValue(currentDate.day());
        ui->spinBox_months->setValue(currentDate.month());
        ui->spinBox_years->setValue(currentDate.year() % 100);

        // Disable spin boxes for days, months, and years
        ui->spinBox_days->setEnabled(false);
        ui->spinBox_months->setEnabled(false);
        ui->spinBox_years->setEnabled(false);
    } else {
        // If RTD checkbox is unchecked, enable spin boxes for days, months, and years
        ui->spinBox_days->setEnabled(true);
        ui->spinBox_months->setEnabled(true);
        ui->spinBox_years->setEnabled(true);
    }
}
