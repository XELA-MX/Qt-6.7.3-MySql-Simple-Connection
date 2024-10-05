#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSql>
#include <QMessageBox>
#include <QResizeEvent>
#include <QFont>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Check db connection
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("HostName");
    db.setDatabaseName("DBNAME");
    db.setUserName("USER");
    db.setPassword("PASS");
    if (!db.open()) {
        qDebug() << "Error: " << db.lastError().text();
        QMessageBox::critical(this, "Error de Conexión", "No se pudo conectar a la base de datos. La aplicación se cerrará.");
        QApplication::exit(1);  // Salir de la aplicación con un código de error
    }else{
        qDebug() << "Conectado a la base de datos!";
    }
    db.close();
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);

    //Password TB
    ui->PasswordTextBox->setEchoMode(QLineEdit::PasswordEchoOnEdit);
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    // Manejo de la base
    QMainWindow::resizeEvent(event);

    // Nuevos valores
    int windowWidth = event->size().width();
    //int windowHeight = event->size().height();

    // TITLE FONT
    QFont font = ui->Title->font();
    font.setPixelSize(windowWidth / 40);
    ui->Title->setFont(font);
    ui->Title->setAlignment(Qt::AlignCenter);
    ui->Title->setFixedWidth(windowWidth);
    ui->Title->setFixedHeight(ui->Title->font().pixelSize() + 12);

    // Username Textbox
    int usernameWidth = windowWidth/1.5;
    int usernameHeight = ui->UsernameTextbox->height();
    int usernameY = (ui->Title->y() + ui->Title->height()) + usernameHeight*2.5;
    int usernameX = (windowWidth/2) - (usernameWidth/2);
    ui->UsernameTextbox->setGeometry(usernameX,usernameY,usernameWidth,usernameHeight);

    // Password TB
    int passY = (ui->UsernameTextbox->y() + ui->PasswordTextBox->height()) + usernameHeight*1.5;
    ui->PasswordTextBox->setGeometry(usernameX,passY,usernameWidth,usernameHeight);

    // Login Button
    int btnWidth = windowWidth/3;
    int btnHeight = ui->loginButton->height();
    int btnY = (ui->PasswordTextBox->y() + ui->loginButton->height()) + ui->loginButton->height()*0.5;
    int btnX = (windowWidth/2) - (btnWidth/2);
    ui->loginButton->setGeometry(btnX,btnY,btnWidth,btnHeight);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool checkIfUserIsInUse(QString user){
    QSqlDatabase conn = QSqlDatabase::database(); // Con por defecto
    if(conn.isOpen()){
        qDebug() << "Hay una conexión abierta!";

        // Abrir la conexión
        QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
        db.setHostName("HOST");
        db.setDatabaseName("DB");
        db.setUserName("USER");
        db.setPassword("PASS");

        if (!db.open()) {

            qDebug() << "Error: " << db.lastError().text();
            QMessageBox::critical(0, "Error de Conexión", "No se pudo conectar a la base de datos. La aplicación se cerrará.");
            QApplication::exit(1);  // Salir de la aplicación con un código de error

        }
    }else{
        qDebug() << "Abriendo conexión...";
    }

    // QUERY
    QSqlQuery query;
    query.prepare("SELECT * FROM UserDB WHERE Username = :user");
    query.bindValue(":user" , user);
    // Ejecutar query
    if(query.exec()){
        if(query.size() > 0){
            return false;
        }
    }else{
        qDebug() << "Error en la consulta: " << query.lastError().text();
        QMessageBox::critical(0,"Error en la consulta!" , "Hubo un error al ejecutar la consulta\n\nVerifica la consulta o intentalo de nuevo!");
        return false;
    }

    return true;
}

void MainWindow::on_loginButton_clicked()
{
    qDebug() << "Login Process Init!";

    // Verificar que no estén vacías
    if(ui->PasswordTextBox->text().isEmpty() || ui->UsernameTextbox->text().isEmpty()){
        QMessageBox::critical(0,"Error","Asegúrate de llenar todas las credenciales!");
        qDebug() << "Credenciales vacías!";
        return;
    }

    // Set enablec ocntrols to false
    ui->PasswordTextBox->setEnabled(false);
    ui->UsernameTextbox->setEnabled(false);

    // Abrir la conexión
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("HOSTNAME");
    db.setDatabaseName("DB");
    db.setUserName("USER");
    db.setPassword("PASS");

    if (!db.open()) {

        qDebug() << "Error: " << db.lastError().text();
        QMessageBox::critical(0, "Error de Conexión", "No se pudo conectar a la base de datos. La aplicación se cerrará.");
        QApplication::exit(1);  // Salir de la aplicación con un código de error

    }

    // VER SI EXISTE EL USUARIO
    QSqlQuery query;
    query.prepare("SELECT * FROM UserDB WHERE Username = :user AND Password = :pass");
    query.bindValue(":user", ui->UsernameTextbox->text());
    query.bindValue(":pass" , ui->PasswordTextBox->text());

    // Ejecutar la query:
    if(query.exec()){

        if(query.next() && query.size() > 0){

            qDebug() << "Se han encontrado valores!!";

            if(query.size() > 1){
                qDebug() << "Es probable que el usuario este duplicado!";
                QMessageBox::critical(0,"Usuario duplicado!","El usuario parece estar duplicado!");
                QApplication::exit(1);
            }

            // No esta duplicado
            QMessageBox::information(0,"Éxito!" , "El usuario se ha enconytado exitosamente");
            ui->PasswordTextBox->text().clear();
            ui->UsernameTextbox->text().clear();
            ui->PasswordTextBox->setEnabled(true);
            ui->UsernameTextbox->setEnabled(true);
        }else{
            qDebug() << "No se han encontrado las credenciales!";

            // Preguntar al usuario
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this,"Usuario no existente","El usuario o contraseña son incorretcos\n\nO no existen!\n\n¿Quieres Registrarte?",
                                          QMessageBox::Yes | QMessageBox::No);

            // Manejar la respuesta
            if(reply == QMessageBox::No){

            }else{
                qDebug() << "Comprobar si el nombre de usuario ya existe";

                // Función
                bool itRanGood = checkIfUserIsInUse(ui->UsernameTextbox->text());
                if(!itRanGood){
                    qDebug() << "Error , ya existe el usuario O error en la consulta!";
                    QMessageBox::critical(0,"No Disponible" , "Hay de 2:\n\n1. El usuario no esta disponible \n\n"
                                                              "2.Hubo un error en la consulta!\n\nIntentalo de nuevo o contactame!");

                }else{
                    qDebug() << "Registrando el usuario...";

                    // El usuario esta disponible
                    QSqlQuery insertUser;
                    insertUser.prepare("INSERT INTO UserDB (Username,Password) VALUES (:user,:pass)");
                    insertUser.bindValue(":user" , ui->UsernameTextbox->text());
                    insertUser.bindValue(":pass", ui->PasswordTextBox->text());
                    if(insertUser.exec()){
                        qDebug() << "Se ha registrado!";
                        QMessageBox::information(this,"Éxito!" , "Se ha registrado el usuario!");
                    }else{
                        qDebug() << "Error registrando el usuario!";
                        QMessageBox::critical(0,"No Disponible" , "Hay de 2:\n\n1. El usuario no esta disponible \n\n"
                                                                  "2.Hubo un error en la consulta!\n\nIntentalo de nuevo o contactame!");
                    }
                }
            }

            ui->PasswordTextBox->setEnabled(true);
            ui->UsernameTextbox->setEnabled(true);

        }

    }else{

        qDebug() << "Error en la consulta: " << query.lastError().text();
        QMessageBox::critical(0,"Error en la consulta!" , "Hubo un error al ejecutar la consulta\n\nVerifica la consulta o intentalo de nuevo!");
        ui->PasswordTextBox->setEnabled(true);
        ui->UsernameTextbox->setEnabled(true);
    }
    db.close();
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
}

