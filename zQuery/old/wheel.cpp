// Колесо мыши. ----------------------------------------------------------------
//------------------------------------------------------------------------------
void WEdit::wheelEvent(QWheelEvent *evt) {

    if(evt->modifiers() & Qt::ControlModifier) {

        QFont fnt = ui->edSql->font();

        if(evt->angleDelta().y() > 0){
            fnt.setPointSize(fnt.pointSize() + 1);
            FNC << "fnt.pointSize() + 1";
        }

        if(evt->angleDelta().y() < 0){
            fnt.setPointSize(fnt.pointSize() - 1);
            FNC << "fnt.pointSize() - 1";
        }

        ui->edSql->setFont(fnt);

        evt->accept();
    }// if(evt->modifiers() & Qt::ControlModifier)
    else {
        QWidget::wheelEvent(evt);
    }
}// wheelEvent
