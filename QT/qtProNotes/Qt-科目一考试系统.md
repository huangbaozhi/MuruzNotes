# QT-驾校科目一考试系统

## 1、需求分析

## 2、登录界面
![image](https://huangbaozhi.github.io/Picx/20231216/image.22jnijppag74.webp)
## 3、验证邮箱地址
在登录按钮`on_loginBtn_clicked()`槽中实现
```c++
void LoginDialog::on_loginBtn_clicked()
{
    //QMessageBox::information(this, "提示", "槽方法调用啦！");

    // 正则验证邮箱地址 用户名@域名 653975671@qq.com
    // 表示符解释：^表示规则字符串的开始 $表示规则字符串的结束
    // +表示匹配次数>=1次 *表示匹配任意次数(可为0次） {n,m}表示匹配次数至少n次，至多m次

    // 验证邮箱地址，验证邮箱
    QRegExp rx("^[A-Za-z0-9]+([_\.][A-Za-z0-9]+)*@([A-Za-z0-9\-]+\.)+[A-Za-z]{2,6}$");
    bool res = rx.exactMatch(ui->accountEdit->text());

    if(!res) // 匹配不成功
    {
        QMessageBox::information(this, "提示", "非法的邮箱地址，请你重新输入!");
        ui->accountEdit->clear();
        ui->codeEdit->clear();
        ui->accountEdit->setFocus();
    }
    else
    {
        //QMessageBox::information(this, "提示", "欢迎登录科目一考试系统!");
        QString filename;           // 账号密码数据文件
        QString strAccInput;        // 用户输入的账号
        QString strCode;            // 用户输入的密码
        QString strLine;            // 每次读取的一行数据
        QStringList strList;        // 保存分割读取的一行数据

        filename = "account.txt";
        strAccInput = ui->accountEdit->text();
        strCode = ui->codeEdit->text();

        QFile file(filename);
        QTextStream stream(&file);
        qDebug()<<"[文件路径 ]= "<<filename;
        if(file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            while(!stream.atEnd()){
            strLine = stream.readLine();
            strList = strLine.split(",");
            qDebug()<<"[账号]= "<<strList.at(0);
            qDebug()<<"[密码]= "<<strList.at(1);
            if(strAccInput == strList.at(0))    // 账号匹配成功
                if(strCode == strList.at(1))    // 密码匹配成功
                {
                    QMessageBox::information(this,"提示","欢迎进入科目一考试系统！");
                    file.close();
                    done(Accepted);// 关闭当前窗体，以指定方式返还
                    return;
                }else{
                    QMessageBox::information(this,"提示","您输入的密码有误，请重新输入！");
                    ui->codeEdit->clear();
                    ui->codeEdit->setFocus();
                    file.close();
                    return;
                }
            }

            QMessageBox::information(this,"提示","您输入的账号有误，请重新输入11！");
            ui->accountEdit->clear();
            ui->codeEdit->clear();
            ui->accountEdit->setFocus();
            file.close();
            return;
        }
        else
        {
            QMessageBox::information(this,"提示","读取账号数据文件失败！");
            return;
        }
    }
}
```

## 4、验证账号密码
```c++
while(!stream.atEnd()){
    strLine = stream.readLine();
    strList = strLine.split(",");
    qDebug()<<"[账号]= "<<strList.at(0);
    qDebug()<<"[密码]= "<<strList.at(1);
    if(strAccInput == strList.at(0))    // 账号匹配成功
        if(strCode == strList.at(1))    // 密码匹配成功
        {
            QMessageBox::information(this,"提示","欢迎进入科目一考试系统！");
            file.close();
            done(Accepted);// 关闭当前窗体，以指定方式返还
            return;
        }else{
        QMessageBox::information(this,"提示","您输入的密码有误，请重新输入！");
        ui->codeEdit->clear();
        ui->codeEdit->setFocus();
        file.close();
        return;
    }
}
```
## 5、密码隐藏
在构造函数中实现：
```c++
setWindowTitle("考试已用时：0分0秒");
setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
resize(800,900);

initTimer();
```

考试计时：
```c++
void ExamDialog::initTimer()
{
    m_timerGo = 0;// 初始时间为0
    m_timer = new QTimer(this);
    m_timer->setInterval(1000);
    m_timer->start();
    connect(m_timer,SIGNAL(timeout()),this,SLOT(freshTime()));
}
```

## 6、考试时间

```c++
void ExamDialog::freshTime()
{
    m_timerGo++;    // 刷新考试用时
    // 计算分钟
    QString min = QString::number(m_timerGo / 60);
    QString sec = QString::number(m_timerGo % 60);
    setWindowTitle("考试已用时：" + min + "分" +sec + "秒");
}
## 7、布局题库
```c++
void ExamDialog::initLayout()
{
    m_layout = new QGridLayout(this);
    m_layout->setSpacing(10);
    m_layout->setHorizontalSpacing(10);
    m_layout->setVerticalSpacing(10);
}
```

```c++
bool ExamDialog::hasNoSelect()
{
    int radioSelects = 0;
    for(int i = 0; i < 8; i++){
        if(m_btnGroups[i]->checkedButton())
            radioSelects++;
    }

    if(radioSelects != 8)
        return true;

    int checkSelects = 0;
    for(int i = 0; i < 4; i++)
    {
        if(m_checkBtns[i]->isChecked())
            checkSelects++;
    }

    if(checkSelects == 0 || checkSelects == 1)
        return true;

    if(!m_radioA->isChecked() && m_radioB->isChecked())
        return true;

    return false;
}

void ExamDialog::initButtons()
{
    QStringList strList = {"A","B","C","D"};
    for(int i = 0; i < 10; i++)
    {
        // 题目标签
        m_titleLables[i] = new QLabel(this);
        m_titleLables[i]->setText("第" + QString::number(i+1) + "题");
        m_layout->addWidget(m_titleLables[i],1,i);

        // 判断题
        if(i == 9){
            m_radioA = new QRadioButton(this);
            m_radioB = new QRadioButton(this);

            m_radioA->setText("正确");
            m_radioB->setText("错误");

            m_layout->addWidget(m_radioA,2,9);
            m_layout->addWidget(m_radioB,3,9);

            m_btnGroups[8] = new QButtonGroup(this);
            m_btnGroups[8]->addButton(m_radioA);
            m_btnGroups[8]->addButton(m_radioB);
            break;
        }

        if(i < 8) m_btnGroups[i] = new QButtonGroup(this);

        // 选择题
        for(int j = 0; j < 4; j++)
        {
            // 多选题
            if(i == 8){
                m_checkBtns[j] = new QCheckBox(this);
                m_checkBtns[j]->setText(strList.at(j));
                m_layout->addWidget(m_checkBtns[j],2+j,8);
            }else{
                // 单项选择题
                m_radioBtns[4 * i + j] = new QRadioButton(this);
                m_radioBtns[4 * i + j]->setText(strList.at(j));
                m_layout->addWidget(m_radioBtns[4 * i + j],2+j,i);

                m_btnGroups[i]->addButton(m_radioBtns[4 * i + j]);
            }
        }
    }

    QPushButton *submitBtn = new QPushButton(this);
    submitBtn->setText("提交");
    submitBtn->setFixedSize(100,35);
    connect(submitBtn,SIGNAL(clicked(bool)),this,SLOT(getScore()));
    m_layout->addWidget(submitBtn,6,9);
}
```

## 7、提交分数

```c++
void ExamDialog::getScore()
{
    if(hasNoSelect()){
        QMessageBox::information(this,"提示","您有未完成的题目，请完成考试！","是");
        return;
    }

    int scores = 0;
    for(int i = 0; i < 10; i++)
    {
        // 单选题计分
        if(i < 8)
            if(m_btnGroups[i]->checkedButton()->text() == m_answerList.at(i))
                scores += 10;

        // 多项选择题计分
        if(i == 8){
            QString answer = m_answerList.at(i);
            bool hasA = false;
            bool hasB = false;
            bool hasC = false;
            bool hasD = false;

            if(answer.contains("A") ) hasA = true;
            if(answer.contains("B") ) hasB = true;
            if(answer.contains("C") ) hasC = true;
            if(answer.contains("D") ) hasD = true;

            bool checkA = m_checkBtns[0]->checkState();
            bool checkB = m_checkBtns[1]->checkState();
            bool checkC = m_checkBtns[2]->checkState();
            bool checkD = m_checkBtns[3]->checkState();

            if(hasA != checkA) continue;
            if(hasB != checkB) continue;
            if(hasC != checkC) continue;
            if(hasD != checkD) continue;

            scores += 10;
        }

        // 判断题计分
        if(i == 9){
            if(m_btnGroups[8]->checkedButton()->text() == m_answerList.at(i))
                scores += 10;
        }
    }

    QString str = "您的分数是：" + QString::number(scores) + "分，是否重新考试？";
    int res = QMessageBox::information(this,"提示",str,QMessageBox::Yes | QMessageBox::No);
    if(res == QMessageBox::Yes)
        return;
    else
        close();
}

```
