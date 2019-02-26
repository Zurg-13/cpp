FMain.h:
QList<QTcpSocket*> sct; //todo: А оно вообще надо ???

FMain.cpp:
FMain::~FMain() {
    for(QList<QTcpSocket*>::iterator it = sct.begin(); it != sct.end(); it++) {
        QTextStream out(*it);
        out << QDateTime::currentDateTime().toString() << endl;
        (*it)->close();  (*it)->deleteLater();
        sct.erase(it);
    }// it
    tcp->close();

    delete ui; ui = nullptr;
}// ~FMain

void FMain::close_socket(void) {
    QTcpSocket *socket = ((QTcpSocket*)sender());

    log(tr("Сокет закрыт.")
      + QString::number((ulong)socket)
      + " (" + socket->peerAddress().toString() + ")", Qt::lightGray);

    // todo: Пробная, медленная реализация c квадратичной сложностью.
    QtConcurrent::map(sct, [&](const QTcpSocket* element) -> void {
            if(element != socket) return;
            else { sct.removeOne(socket); } });

}// close_socket







