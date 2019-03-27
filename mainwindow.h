#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include"qcustomplot.h"
#include <QMainWindow>
#include<vector>
using std::vector;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void prepareMenu( const QPoint & pos );
    void removeit();
    void addit();
    void closeEvent(QCloseEvent *event);
    void on_pushButton_2_clicked();
    void two(QList<QTreeWidgetItem*>, int &);
    void on_treeWidget_clicked(const QModelIndex &index);

    void on_treeWidget_doubleClicked(const QModelIndex &index);

    void on_action_triggered();
    void my_draw(QString, int);
    void on_action_2_triggered();

    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

private:
    Ui::MainWindow *ui;
    QModelIndex ind;
    QTreeWidgetItem *my_item;
    vector<vector<QString> > vector_for_tree;
    vector<int>int_vec;
    int proj;
    int formul;
    QHash<QTreeWidgetItem*, QTreeWidgetItem*> kombo_graphs;
    QMap<QTreeWidgetItem*, QString> graphs;
};

#endif // MAINWINDOW_H
