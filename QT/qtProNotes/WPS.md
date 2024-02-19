# Qt-WPS
### 1、新建文档
#### 1.1 项目
![!\[Alt text\](image.png)](<截屏2023-12-18 17.29.16.png>)
#### 1.2 设计页面
![Alt text](<截屏2023-12-18 17.16.07.png>)
#### 1.3 项目运行
![Alt text](<截屏2023-12-18 17.31.21.png>)
#### 1.4 代码
##### 新建文档
MainWindow类：
```c++
void MainWindow::docNew()
{
    ChildWnd *childwnd = new ChildWnd;
    ui->mdiArea->addSubWindow(childwnd);
    connect(childwnd,SIGNAL(copyAvailable(bool)),ui->cutAction,SLOT(setEnabled(bool)));
    connect(childwnd,SIGNAL(copyAvailable(bool)),ui->copyAction,SLOT(setEnabled(bool)));

    childwnd->newDoc();
    childwnd->show();
}
```

ChildWnd类：
```c++
void ChildWnd::newDoc()
{
    static int wndSeqNum = 1;
    m_CurDocPath = QString("WPS 文档 %1")
                       .arg(wndSeqNum++);

    // 设置窗体标题，文档改动后名称后加"*"号标识
    setWindowTitle(m_CurDocPath + "[*]" + "- Qt-WPS");
    connect(document(),SIGNAL(contentsChanged()),this,SLOT(docBeModified()));
}

void ChildWnd::docBeModified()
{
    setWindowModified(document()->isModified());
}

```

### 2、活动子窗口
