#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QProcess>
#include <QString>
#include <QMessageBox>
using namespace std;

vector< set <string> > algorithm(int timeSpan = 2, double t_threshold = 2, double pi_threshold = 0.5);
vector< vector<string>> inputData;
map<string, int> featureNum;  //存储实例个数
vector<set <string> > ans;
vector<SpatioNode> allInstance; //时空模式专用，保存所有拥堵实例
vector<Road> allRoad; //时空模式专用，保存所有道路。
map<string, vector<string>> roadNeighbor ;//存储道路临近关系
//map<SpatioNode*, SpatioNode*> insNeighborMap;
vector<pair<SpatioNode*, SpatioNode*> > insNeighborMap;
/*============================= some functions which SPCCP-Miner algotirhm need *=================================================*/
// to check the data which user upload whether legal.
bool checklegal(){
    if(inputData.size() == 0) return false;
    string str1 = inputData[0][0];
    for(auto c: str1){
        if(!isalpha(c)){
            return false;
        }
    }
    string str2 = inputData[inputData.size()-1][0];
    if(!isdigit(str2[str2.size()-1])){
        return false;
    }
    return true;
}
//pourn low-size SPCCPs if these SPCCPs are subset of new SPCCP
void pournLowSizeSPCCP(const set<string>& strs){
    auto it = ans.begin();
    while(it != ans.end()){
        set<string> unionSet;
        set<string>& temp = *it;
        set_union(temp.begin(), temp.end(), strs.begin(), strs.end(), inserter(unionSet, unionSet.begin()));
        if(unionSet == strs){
            it = ans.erase(it);
        }else{
            ++it;
        }
    }
}
//Check whether two instances satisfy the neighbor relationship
bool checkNeighbor(SpatioNode& node1, SpatioNode& node2, int timeSpan, int t_threshold){
//    cout << node1.getInsName() << " " << node2.getInsName() << endl;
    string roadName1 = node1.roadName();
    string roadName2 = node2.roadName();
    auto it = find(roadNeighbor[roadName1].begin(),roadNeighbor[roadName1].end(),roadName2);
    if(it != roadNeighbor[roadName1].end()){
        if(abs(node1.getTimeSpan()-node2.getTimeSpan()) * timeSpan <= t_threshold){
            return true;
         }
    }
    return false;
}
// a Table instance, join-based algorithm need.
struct Table
{
    int _size;
    set<string> _str;
    vector<set<SpatioNode > > _table;
};
//check a candidate SPCCP whether prevalent or not
bool checkPrivalent(Table t, double piPre){
    // map<string, int> counter;
    map<string, set<string> > counter;
    for(auto it = t._table.begin(); it != t._table.end(); it++){
        set<SpatioNode>& s = *it;
        for(auto sit = s.begin(); sit != s.end(); sit++){
            counter[sit->roadName()].insert(sit->getInsName());
        }
    }
    double pi = 1.0;
    for(auto it = counter.begin(); it != counter.end(); it++){
        double pr = it->second.size()/(featureNum[it->first]*1.0);
        pi = min(pi, pr);
    }
    if(pi >= piPre){
        return true;
    }else{
        return false;
    }
}
//merge two table
Table mergeTable(Table& table1, Table& table2){
    Table newTable;
    set<string> newSet(table1._str);
    for(auto it = table2._str.begin(); it != table2._str.end(); it++){
        if(it == table2._str.begin()) continue;
        newSet.insert(*it);
    }
    newTable._str = newSet;
    newTable._size = newSet.size();
    vector<set<SpatioNode > >& _table1 = table1._table;
    vector<set<SpatioNode > >& _table2 = table2._table;
    for(int i = 0; i < _table1.size(); i++){
        for(int j = 0; j < _table2.size(); j++){
            const SpatioNode& node1 = *(_table1[i].begin());
            const SpatioNode& node2 = *(_table2[j].begin());
            if(node1.getInsName() == node2.getInsName()){
                set<SpatioNode> temp = _table1[i];
                for(auto it = _table2[j].begin(); it != _table2[j].end(); it++){
                    if(it == _table2[j].begin()) continue;
                    temp.insert(*it);
                }
                newTable._table.push_back(temp);
            }
        }
    }
    return newTable;
}
//Check if the input to the congestion instance
bool inputCheck(int i){
    int size = inputData[i][0].size();
    if(isdigit(inputData[i][0][size - 1])){
        return true;
    }
    return false;
}

void trim(string& bufStr){    //该函数用于数据清洗时去除前后多余空格
    int loc = bufStr.find_first_not_of(' ');
    if(loc != -1){
        bufStr = bufStr.substr(loc, bufStr.size()-loc);
    }
    loc = bufStr.find_last_not_of(' ');
    if(loc != -1){
        bufStr = bufStr.substr(0, loc+1);
    }
    loc = bufStr.find_last_not_of('\n');
    if(loc != -1){
        bufStr = bufStr.substr(0, loc+1);
    }
    loc = bufStr.find_first_not_of('\n');
    if(loc != -1){
        bufStr = bufStr.substr(loc, bufStr.size()-loc);
    }
}
//clear all container
void clearData(){
    insNeighborMap.clear();
    roadNeighbor.clear();
    allRoad.clear();
    allInstance.clear();
    ans.clear();
    featureNum.clear();
}
//open a messagebox
void MainWindow::on_buttonSave_clicked(){
    QFileDialog dlg(this);

    //获取内容的保存路径
    QString fileName = dlg.getSaveFileName(this, tr("Save As"), "./", tr("Text File(*.txt)"));

    if( fileName == "" )
    {
        return;
    }

    //内容保存到路径文件
    QFile file(fileName);
    //以文本方式打开
    if( file.open(QIODevice::WriteOnly | QIODevice::Text) )
    {
        QTextStream out(&file); //IO设备对象的地址对其进行初始化

        out << ui->textBrowser->toPlainText() << endl; //输出

        QMessageBox::information(this, tr("Finish"), tr("Successfully save the file!"));

        file.close();
    }
    else
    {
        QMessageBox::warning(this, tr("Error"), tr("File to open file!"));
    }
}
/*================================================================================*/


/*============= the interface of SPCCP-Miner ==============================*/
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(0);
//    ui->progressBar->setMinimum(0);
//    ui->progressBar->setMaximum(100);
//    ui->progressBar->setValue(0);
    connect(ui->actionopen_2, &QAction::triggered , [=]()->void{
        QString path = QFileDialog::getOpenFileName(this,"open file", "../demo","*.txt");
        QFile file(path);
        file.open(QIODevice::ReadOnly);
        QByteArray arrary;
        while(!file.atEnd()){
            QByteArray tempArry = file.readLine();
            arrary += tempArry;
            string str(tempArry);
            char* s = new char[str.size() + 1];
            strcpy(s, str.c_str());
            char* p = strtok(s, " ");
            vector<string> words;
            while(p) {
                words.push_back(p);
                p = strtok(NULL, " ");
            }
            for(string& s: words){
                trim(s);
            }
            inputData.push_back(words);  
            ui->textBrowser_2->setText(arrary);
        }
        for(int i = 0; i < inputData.size(); i++){
            string& s = inputData[i][inputData[i].size()-1];
            if(isdigit(s[s.size()-1])) continue;
            s = s.substr(0, s.size()-1);
        }
        file.close();
    });

    write* w = new write(this);
    connect(ui->actionopen_3, &QAction::triggered, this, [=](){
        w->show();
    });

    connect(ui->reset_pushButton, &QPushButton::clicked, [&](){
        ui->textBrowser->clear();
        ui->textBrowser_2->clear();
        ui->textEdit->clear();
        clearData();
        inputData.clear();
    } );


    connect(ui->start_pushButton, &QPushButton::clicked, [&](){
        int timeSpan = ui->timeSpanlineEdit->text().toInt();
        //double f_threshold = ui->f_thresholdlineEdit->text().toDouble();
        int t_threshold = ui->t_thresholdlineEdit->text().toInt();
        double pi_threshold = ui->pi_thresholdlineEdit->text().toDouble();
        if(ui->textBrowser->toPlainText().size() != 0){
            ui->textBrowser->clear();
        }
        if(ui->tabWidget->currentIndex() == 1){
            clearData();
            inputData.clear();
            string str = ui->textEdit->toPlainText().toStdString();
            vector<string> lineStr;
            string temp;
            for(int i = 0; i < str.size(); i++){
                if(str[i] == '\n' || i == str.size()-1){
                    if(i == str.size()-1) temp.push_back(str[i]);
                    lineStr.push_back(temp);
                    temp.clear();
                }else{
                    temp.push_back(str[i]);
                }
            }
            vector<string> tempV;
            for(int i = 0; i < lineStr.size(); i++){
                if(lineStr[i][0] == '/' || lineStr[i][0] == ' ') continue;
                string tempStr;
                for(int j = 0; j < lineStr[i].size(); j++){
                    if(lineStr[i][j] == ' ' || j == lineStr[i].size()-1){
                        if(j == lineStr[i].size()-1) tempStr.push_back(lineStr[i][j]);
                        tempV.push_back(tempStr);
                        tempStr.clear();
                    }else{
                        trim(tempStr);
                        tempStr.push_back(lineStr[i][j]);
                    }
                }
                inputData.push_back(tempV);
                tempV.clear();
            }
        }
        if(!checklegal()){
            inputData.clear();
            QMessageBox::warning(this, tr("Error"), tr("Please input data in the correct format"));
            return;
        }
        vector< set<string> > ans = algorithm(timeSpan, t_threshold, pi_threshold);
        QFont fontAns = QFont("Microsoft YaHei",20,2);
        fontAns.setFamily("微软雅黑");//字体
        fontAns.setPixelSize(30);
        fontAns.setPointSize(20);
        ui->textBrowser->setFont(fontAns);
        for(int i = 0; i < ans.size(); i++){
            for(auto it = ans[i].begin(); it != ans[i].end(); it++){
                string str = *it;
                ui->textBrowser->insertPlainText(QString::fromStdString(str));
                if(it == ans[i].begin()) ui->textBrowser->insertPlainText("->");
                ui->textBrowser->insertPlainText(" ");
            }
//            if(ans[i].size() == 2){
//                ui->textBrowser->insertPlainText("🚦");
//            }else{
//                ui->textBrowser->insertPlainText("👮");
//            }
            ui->textBrowser->append("");
        }
        clearData();
    });
    connect(ui->commandLinkButton, &QCommandLinkButton::clicked, [&](){
       QProcess::startDetached("explorer https://github.com/miaomiaoCharles/SPCCP-Miner");
    });
    connect(ui->commandLinkButton_2, &QCommandLinkButton::clicked, [&](){
       QProcess::startDetached("explorer https://youtu.be/1RuCg0LegbM");
    });
    connect(ui->actionFile_Mode, &QAction::triggered, [&](){
        ui->tabWidget->setCurrentIndex(0);
    });
    connect(ui->actionsave, &QAction::triggered, [&](){
        this->on_buttonSave_clicked();
    });
    connect(ui->actionWrite_Mode, &QAction::triggered, [&](){
        ui->tabWidget->setCurrentIndex(1);
    });
    connect(ui->actioninstruction, &QAction::triggered, [&](){
        QProcess::startDetached("explorer https://github.com/miaomiaoCharles/SPCCP-Miner");
    });
    connect(ui->actionhelp_video, &QAction::triggered, [&](){
        QProcess::startDetached("explorer https://youtu.be/1RuCg0LegbM");
    });
    connect(ui->save_pushButton, &QPushButton::clicked,[&](){
        this->on_buttonSave_clicked();
    });
//    connect(ui->map_pushButton, &QPushButton::clicked, [&](){
//        QWebEngineView* view = new QWebEngineView(parentWidget());
//        view->load(QUrl("D:/Documents/code/demo/BaiDuMap.html")); // D://baidu.html就是自己刚创建的
//        view->show(); // 显示百度地图
//    });

}

MainWindow::~MainWindow()
{
    delete ui;
}

/*========================================================================================================================*/


/*============================== algorithm of SPCCP========================================================================*/
vector< set <string> > algorithm(int timeSpan, double t_threshold, double pi_threshold){
    int i = 0;
    for(i = 0; i < inputData.size(); i++){
        trim(inputData[i][0]);
        if(inputCheck(i)) break;
        Road r(inputData[i][0]);
        if(inputData[i].size() <= 1) roadNeighbor.insert({inputData[i][0], {}});
        for(int j = 1; j < inputData[i].size(); j++){
            trim(inputData[i][j]);
            r._neighbor.push_back(inputData[i][j]);
            roadNeighbor[inputData[i][0]].push_back(inputData[i][j]);
        }
        allRoad.push_back(r);
    }
    //开始接收拥塞实例
    for(; i < inputData.size(); i++){
        for(int j = 0; j < inputData[i].size(); j++){
            allInstance.push_back(SpatioNode(inputData[i][j]));
        }
    }
    //将实例放入对应的Road里去
    for(int i = 0; i < allRoad.size(); i++){
        Road& r = allRoad[i];
        for(SpatioNode& ins: allInstance){
            if(ins.roadName() == r.name()){
                r.congestionTimes.push_back(&ins);
                ins.feature = &(allRoad[i]);
            }
        }
    }

    for(SpatioNode& insNode1: allInstance){
        for(SpatioNode& insNode2: allInstance){
            if(insNode1.roadName() != insNode2.roadName() && checkNeighbor(insNode1, insNode2, timeSpan, t_threshold)){
                insNeighborMap.push_back({&insNode1, &insNode2});
            }
        }
    }
    for(auto ins: allInstance){
        if(featureNum.find(ins.roadName()) == featureNum.end()){
            featureNum.insert({ins.roadName(), 1});
        }else{
            featureNum[ins.roadName()] ++;
        }
    }
    map< set<string>, Table> tableMap; //存储对应的表实例
    //先计算2阶的情况
    for(int i = 0; i < insNeighborMap.size(); i++){
        set<string> strSet;
        strSet.insert(insNeighborMap[i].first->roadName());
        strSet.insert(insNeighborMap[i].second->roadName());
        if(tableMap.find(strSet) == tableMap.end()){
            Table t;
            t._str = strSet;
            t._size = strSet.size();
            tableMap[strSet] = t;
        }
        set<SpatioNode> tempSet;
        tempSet.insert(*(insNeighborMap[i].first));
        tempSet.insert(*(insNeighborMap[i].second));
        tableMap[strSet]._table.push_back(tempSet);
    }
    vector<Table> prevalentTable;
    for(auto it = tableMap.begin(); it != tableMap.end(); it++){
        if(checkPrivalent(it->second, pi_threshold)){
            ans.push_back(it->first);
            Table& sucessTable = it->second;
            prevalentTable.push_back(sucessTable);
        }
    }//size-2 complete
    int k = 2;
    for( ; prevalentTable.size() > 2; k++){
        vector<Table> newPrevalentTable;
        for(int i = 0; i < prevalentTable.size()-1; i++){
            Table& table1 = prevalentTable[i];
            for(int j = i+1; j < prevalentTable.size(); j++){
                Table& table2 = prevalentTable[j];
                if(*(table1._str.begin()) == *(table2._str.begin())){
                    Table newTable = mergeTable(table1, table2);
                    if(checkPrivalent(newTable, pi_threshold)){
                        pournLowSizeSPCCP(newTable._str);
                        ans.push_back(newTable._str);
                        newPrevalentTable.push_back(newTable);
                    }
                }
            }
        }
        prevalentTable = newPrevalentTable;
    }
    return ans;
}

/*=====================================================================================================*/
