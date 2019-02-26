FMain.h:
QList<QTcpSocket*> sct; //todo: � ��� ������ ���� ???

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

    log(tr("����� ������.")
      + QString::number((ulong)socket)
      + " (" + socket->peerAddress().toString() + ")", Qt::lightGray);

    // todo: �������, ��������� ���������� c ������������ ����������.
    QtConcurrent::map(sct, [&](const QTcpSocket* element) -> void {
            if(element != socket) return;
            else { sct.removeOne(socket); } });

}// close_socket







