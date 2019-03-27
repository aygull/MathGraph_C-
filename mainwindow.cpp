#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QString>
#include <string>
//#include<QVector>
#include"math.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeWidget,&QTreeWidget::customContextMenuRequested,this,&MainWindow::prepareMenu);
    ui->widget->xAxis->setLabel("x");
    ui->widget->yAxis->setLabel("y");
    ui->widget->setInteraction(QCP::iRangeZoom,true);
    ui->widget->addGraph();
    ui->widget->setInteraction(QCP::iRangeDrag, true);
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::closeEvent(QCloseEvent *event){

}
void MainWindow::my_draw(QString text, int my_index){
    //ui->widget->clearGraphs();

    //QString text = ui->lineEdit->text();
    QString strfordigit;

    text.remove(' ');
    text.remove('*');
    double delta, determinat, track, a11,a12,a13,a22,a23,a33,pol, lambada1, lambada2, discr;
    a11=0;
    a12=0;
    a13=0;
    a22=0;
    a23=0;
    a33=0;
    QString forchar;
    double dob;
    QList<QString> list;
    bool oper = true;
    list<<"x"<<"x^2"<<"y"<<"y^2"<<""<<"xy";
    for(int i=0; i<text.size();i++){
        if ((text[i].isDigit()) || (text[i]=='.')){
            if(i != 0 && text[i-1]=='^'){
                if(text[i]!='2'){
                qDebug()<< "NEVERNO";
                return;
                }
                forchar.append(text[i]);
            }
            else{
                strfordigit.append(text[i]);
            }
        }
        else if((text[i]=='x')||(text[i]=='y')||(text[i]=='^')){
            forchar.append(text[i]);

        }

        else if ((text[i]=='+')||(text[i]=='-')||(text[i]=='=')){
            dob=strfordigit.toDouble();
            if(!(forchar.isEmpty())&&(strfordigit.isEmpty()))
                dob=1;
            switch(list.indexOf(forchar)){
            case 0:{
                if (oper)
                    a13+=dob;
                else
                    a13-=dob;
                break;
            }
            case 1:{
                if (oper)
                    a11+=dob;
                else
                    a11-=dob;
                break;
            }
            case 2:{
                if (oper)
                    a23+=dob;
                else
                    a23-=dob;
                break;
            }
            case 3:{
                if (oper)
                    a22+=dob;
                else
                    a22-=dob;
                break;
            }
            case 4:{
                if (oper)
                    a33+=dob;
                else
                    a33-=dob;
                break;
            }
            case 5:{
                if (oper)
                    a12+=dob;
                else
                    a12-=dob;
                break;
            }
            default:{
                qDebug()<<"Неверный ввод";
                return;
            }
            }
            if (text[i] == '-'){
                oper = false;
            }
            else
                oper=true;
            strfordigit="";
            forchar="";
        }
        else{
            qDebug()<<"Неверный ввод";
            return;
        }
    }
//    qDebug()<<text;
    a12=a12/2;
    a13=a13/2;
    a23=a23/2;
    delta=a11*a22*a33+a12*a13*a23+a13*a23*a12-a13*a13*a22-a23*a23*a11-a12*a12*a33;
    determinat=a11*a22-a12*a12;
    track=a11+a22;
    pol=a11*a33-a13*a13+a22*a33-a23*a23;
    ui->textBrowser->setText("Инвараинты\nДельта=" + QString::number(delta) + "\nD= " + QString::number(determinat) + "\nI= " + QString::number(track));
//    qDebug()<<delta<<determinat<<track;
//    qDebug()<<a11<<a22<<a12<<a13<<a23<<a33;
    discr=track*track-4*determinat;
    lambada1=(track+sqrt(discr))/2;
    lambada2=(track-sqrt(discr))/2;
    double tmp;
    if(lambada2 > lambada1){
        tmp = lambada2;
        lambada2 = lambada1;
        lambada1 = tmp;
    }

    if ((determinat>0) && (delta*track<0)){
     //эллипс}
        ui->widget->addGraph();
        QCPItemEllipse *ellipse = new QCPItemEllipse(ui->widget);
        double x0,y0;
        x0=(a12*a23-a13*a22)/determinat;
        y0=(a13*a12-a11*a23)/determinat;
        double a = sqrt((0-delta)/(lambada1*lambada2*lambada2));
        double b = sqrt((0-delta)/(lambada1*lambada1*lambada2));
        ellipse->topLeft->setCoords(x0-a,y0+b);
        ellipse->bottomRight->setCoords(x0+a,y0-b);
        ellipse->setVisible(true);
        ellipse->setPen(QPen(Qt::blue));
        ui->widget->replot();
        ui->widget->graph(my_index)->setLineStyle((QCPGraph::lsNone));
        ui->widget->graph(my_index)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 3));
    }
    if(determinat < 0){
      float disc;
      std::vector<double> x,y;
      ui->widget->addGraph();
      for (double i=-10; i<=10;i+=0.01){
          disc = (pow((2*a12*i+2*a23),2)-(4*a22*(a11*i*i+2*a13*i+a33)));
          x.push_back(i);
          y.push_back((-(2*(a12*i)+2*a23)+sqrt(disc))/2*a22);
          x.push_back(i);
          y.push_back((-(2*(a12*i)+2*a23)-sqrt(disc))/2*a22);
      }
      ui->widget->graph(my_index)->setLineStyle((QCPGraph::lsNone));
      ui->widget->graph(my_index)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 3));
      ui->widget->graph(my_index)->addData(QVector<double>::fromStdVector(x),QVector<double>::fromStdVector(y));
      ui->widget->replot();
    }
    if ((determinat==0)&&(track==1)){//парабола
         std::vector<double> x,y;
         float disc;
         ui->widget->addGraph();
          for (double i=-10; i<=10;i+=0.01){
              disc = (pow((2*a12*i+2*a23),2)-(4*a22*(a11*i*i+2*a13*i+a33)));
              x.push_back(i);
              y.push_back((-(2*(a12*i)+2*a23)+sqrt(disc))/2*a22);
              x.push_back(i);
              y.push_back((-(2*(a12*i)+2*a23)-sqrt(disc))/2*a22);
          }
          ui->widget->graph(my_index)->setLineStyle((QCPGraph::lsNone));
          ui->widget->graph(my_index)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 3));
          ui->widget->graph(my_index)->addData(QVector<double>::fromStdVector(x),QVector<double>::fromStdVector(y));
          ui->widget->replot();
    }
    if ((delta==0)&&(determinat==0)&&(track==0)){
        ui->widget->addGraph();
        QCPItemLine *min1=new QCPItemLine(ui->widget);
        QPen pen(Qt::black); pen.setWidth(2);
      //  min1->setPen(pen);
       // min1->setSelectable(true);
        min1->start->setCoords(0,-a33);
          for(int i=-10; i<=10;i++){
        min1->end->setCoords(i,-a13*i-a33);
          }
          ui->widget->graph(my_index)->setLineStyle((QCPGraph::lsNone));
          ui->widget->graph(my_index)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 3));

    }
    ui->widget->replot();
}

void MainWindow::on_pushButton_clicked()
{
    ui->widget->clearGraphs();
    my_draw(ui->lineEdit->text(), 0);
    QMap<QTreeWidgetItem*, QString>::iterator it = graphs.find(my_item);
    *it = ui->lineEdit->text();
}

inline uint qHash(const QTreeWidgetItem& my_item, uint speed){
    return qHash(my_item.text(0), speed) ^ my_item.data(0,0).toInt();
}

inline bool operator==(const QTreeWidgetItem& my_item, const QTreeWidgetItem& my_item1){
    return my_item == my_item1;
}


void MainWindow::addit(){
    QTreeWidgetItem *topLevelItem = new QTreeWidgetItem();
    bool ok;
    QString ProjName = QInputDialog::getText(0, "Создание новой кривой","Создаем кривую для проекта", QLineEdit::Normal,"Кривая", &ok);
    if (!ok){
        QMessageBox::warning(this, "","Ошибка в вводе");
        return;
    }

    topLevelItem->setText(0,ProjName);
    if (ind.row()==-1){
        ui->treeWidget->addTopLevelItem(topLevelItem);
        graphs.insert(topLevelItem, "x^2-y^2-4=0");
    }
    else{
        if(graphs.contains(ui->treeWidget->currentItem())){
            graphs.remove(ui->treeWidget->currentItem());
            kombo_graphs.insert(ui->treeWidget->currentItem(), topLevelItem);
        }else{
            kombo_graphs.insertMulti(ui->treeWidget->currentItem(), topLevelItem);
        }
        ui->treeWidget->currentItem()->addChild(topLevelItem);
        graphs.insert(topLevelItem, "x^2-y^2-4=0");
    }

}

void MainWindow::two(QList<QTreeWidgetItem*> list, int &index_graph){


    foreach (QTreeWidgetItem* sub_item, list) {
        if(graphs.contains(sub_item)){
           QMap<QTreeWidgetItem*, QString>::iterator it = graphs.find(sub_item);
            my_draw(it.value(), index_graph++);
        }
        else{
            QList<QTreeWidgetItem*> sub_list = kombo_graphs.values(sub_item);
            two(sub_list, index_graph);
        }
    }
}

void MainWindow::removeit(){
    delete ui->treeWidget->currentItem();
}


void MainWindow::prepareMenu( const QPoint & pos )
{
    ind=ui->treeWidget->indexAt(pos);

    QAction *newAct = new QAction ("Добавить", this);
    QAction *newAct2 = new QAction ("Удалить", this);
    connect(newAct, SIGNAL(triggered()), this, SLOT(addit()));
    connect(newAct2, SIGNAL(triggered()), this, SLOT(removeit()));
    QMenu menu(this);
    menu.addAction(newAct);
    if(ind.row()!=-1){
        menu.addAction(newAct2);
    }
    menu.exec(ui->treeWidget->viewport()->mapToGlobal(pos) );
}

void MainWindow::on_pushButton_2_clicked()
{

}

void MainWindow::on_treeWidget_clicked(const QModelIndex &index)
{

}

void MainWindow::on_treeWidget_doubleClicked(const QModelIndex &index)
{

}

void MainWindow::on_action_triggered()
{
    QMessageBox box;
    box.setText("Данная программа рисует графики кривой не выше второго порядка");
    box.exec();
}

void MainWindow::on_action_2_triggered()
{
    QMessageBox box;
    box.setText("Программу выполнил студент 2 курса Якупова Айгуль М8О-212Б-17.");
    box.exec();
}

void MainWindow::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    ui->widget->clearGraphs();
    my_item = item;
    ui->textBrowser->clear();
    if(item->childCount() == 0){
        ui->pushButton->setEnabled(true);
        QMap<QTreeWidgetItem*, QString>::iterator i = graphs.find(item);
        ui->lineEdit->clear();
        ui->lineEdit->setText(i.value());
        ui->widget->clearGraphs();
        my_draw(i.value(), 0);
        ui->widget->replot();
    }
    else{
        ui->lineEdit->clear();
        ui->pushButton->setEnabled(false);
        QList<QTreeWidgetItem*> list = kombo_graphs.values(item);
        ui->widget->clearGraphs();
        int index_graph = 0;
        two(list, index_graph);
        ui->widget->replot();
    }
}
