// BANCO DE DADOS MYSQL:
CREATE database HorasExtras;
use HorasExtras;
CREATE TABLE Funcionarios 
( 
 ID_funcionario INT PRIMARY KEY,
 CargaHoraria INT NOT NULL,
 HorasSemanais INT,
 Cargo VARCHAR(20),
 NomeFuncionario VARCHAR (100) NOT NULL,
HorasExtras INT,
HorasExtrasPendentes INT
); 

CREATE TABLE RegistroHoras 
( 
 IDregistro INT PRIMARY KEY AUTO_INCREMENT,
 DiaSemana INT
);

//USUARIO MYSQL:
CREATE USER 'user'@'localhost' IDENTIFIED BY 'Abcaei12';
GRANT ALL PRIVILEGES ON HorasExtras.* TO 'user'@'localhost';
FLUSH PRIVILEGES;

//COMPILACAO:
gcc main_corrigido.c db_corrigido.c -o programa -lmysqlclient
./programa
