#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QDebug>
#include <QIcon>
#include <QSize>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->textEdit_name_header->setText("filename.h");
    QSize size(36,36);
    QIcon icon;
    icon=QIcon("/home/optimus/Documentos/source_code/qt/bmp_to_code/convert_bmp_to_code/png/open_folder003.png");
    ui->toolButton_open_file->setIcon(icon);
    icon=QIcon("/home/optimus/Documentos/source_code/qt/bmp_to_code/convert_bmp_to_code/png/exit.png");
    ui->pushButton_exit->setIconSize(size);
    ui->pushButton_exit->setIcon(icon);
    icon=QIcon("/home/optimus/Documentos/source_code/qt/bmp_to_code/convert_bmp_to_code/png/checkmark_icon&32.png");
    ui->pushButton_run->setIcon(icon);

}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_toolButton_open_file_clicked()
{
QString filename=QFileDialog::getOpenFileName(
            this,
            tr("Open"),
            "/",
            "All files(*.*);;Text File(*.bmp);;"
            );
QMessageBox::information(this,tr("File Name"),filename);
global_filename=filename;
}

void MainWindow::on_pushButton_exit_clicked()
{
    this->close();
}



void MainWindow::on_pushButton_run_clicked()
{

    QString buff=ui->textEdit_name_header->toPlainText();
    QFile fs_bmp(global_filename);
    QFile fs_header(buff);


    if(!fs_header.open(QIODevice::WriteOnly | QIODevice::Text))return;
    if(!fs_bmp.open(QIODevice::ReadOnly | QIODevice::Text))return;

    QTextStream insert(&fs_header);
    QTextStream stream_reader(&fs_bmp);
    QString name_str_bmp;

    name_str_bmp=ui->textEdit_name_str->toPlainText();

    insert<<"//#include<Graphics\Graphics.h>\r\n";
    insert<<"extern const char ";
    insert<<name_str_bmp;
    insert<<"[];\r\n";
    insert<<"//BITMAP NAME CAN BE CHANGED HERE.\r\n";
    insert<<"//volatile const BITMAP_FLASH ";
    insert<<name_str_bmp;
    insert<<"= {0,L00000};\r\n";
    insert<<"const char ";
    insert<<name_str_bmp;
    insert<<"[]={\r\n";

QString temp_text;
char buff_tmp[7];
char onebyte[3];
buff_tmp[0]=0x00;

stream_reader.seek(0x1C);
fs_bmp.read(onebyte,sizeof(unsigned char));
buff_tmp[1]=onebyte[0];


temp_text=QByteArray::number(buff_tmp[1]);
ui->label_bits_value->setText(temp_text);

stream_reader.seek(0x12);
fs_bmp.read(onebyte,sizeof(unsigned char));
buff_tmp[2]=onebyte[0];



stream_reader.seek(0x13);
fs_bmp.read(onebyte,sizeof(unsigned char));
buff_tmp[3]=onebyte[0];

temp_text=QByteArray::number((unsigned char)(buff_tmp[2]|(buff_tmp[3]<<8)));
ui->label_width_value->setText(temp_text);

stream_reader.seek(0x16);
fs_bmp.read(onebyte,sizeof(unsigned char));
buff_tmp[4]=onebyte[0];


stream_reader.seek(0x17);
fs_bmp.read(onebyte,sizeof(unsigned char));
buff_tmp[5]=onebyte[0];

temp_text=QByteArray::number((unsigned char)(buff_tmp[4]|(buff_tmp[5]<<8)));
ui->label_height_value->setText(temp_text);

    char hex[3];

    sprintf(hex , "%X%X", (buff_tmp[0]>>4)&0xF,buff_tmp[0]&0x0F);
    insert<<"0x"<< hex<<",";

    sprintf(hex , "%X%X", (buff_tmp[1]>>4)&0xF,buff_tmp[1]&0x0F);
    insert<<"0x"<< hex<<",";

    sprintf(hex , "%X%X", (buff_tmp[2]>>4)&0xF,buff_tmp[2]&0x0F);
    insert<<"0x"<< hex<<",";

    sprintf(hex , "%X%X", (buff_tmp[3]>>4)&0xF,buff_tmp[3]&0x0F);
    insert<<"0x"<< hex<<",";

    sprintf(hex , "%X%X", (buff_tmp[4]>>4)&0xF,buff_tmp[4]&0x0F);
    insert<<"0x"<< hex<<",";

    sprintf(hex , "%X%X", (buff_tmp[5]>>4)&0xF,buff_tmp[5]&0x0F);
    insert<<"0x"<< hex<<",";
    insert<<"0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,"<<"//first 18 bytes header"<<"\r\n";

    char qbuff[3];
    int count_bytes=0;
    int counter=0;
    stream_reader.seek(138);

    while(!stream_reader.atEnd()){
        count_bytes++;
        fs_bmp.read(qbuff,sizeof(unsigned char));

        sprintf(hex , "%X%X", (qbuff[0]>>4)&0xF,qbuff[0]&0x0F);
        insert<<"0x"<< hex<<",";

        counter++;
        if(counter>=32){counter=0;insert<<"\r\n";}
    }
    qDebug()<<"bytes:"<<count_bytes<<"\r\n"<<"end";
    insert<<"};\r\n"<<"//read bytes:"<<count_bytes;
    fs_bmp.flush();
    fs_header.close();
    fs_bmp.close();
}
