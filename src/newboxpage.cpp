#include "newboxpage.h"
#include "dbbackgroundworker.h"
#include "distrobox.h"
#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QProgressBar>
#include <QThread>
#include <qboxlayout.h>
#include <qfont.h>
#include <qframe.h>
#include <qlabel.h>
#include <qlayoutitem.h>
#include <qobject.h>

NewBoxPage::NewBoxPage(
    QWidget *parent, std::unordered_map<std::string, std::string> *distroIcons)
    : QWidget(parent), m_distroIcons(distroIcons) {
    // TODO smart pointers and maybe member vars
    // TODO put the form in its own frame separate form the title
    m_images = Distrobox::getAvailableImages();

    QFont font = this->font();
    font.setPixelSize(16);
    this->setFont(font);

    QFont labelFont = this->font();
    labelFont.setPixelSize(20);

    // output messages
    m_outputLabel = new QLabel(" ");
    m_outputLabel->setAlignment(Qt::AlignCenter);
    m_outputLabel->setFont(labelFont);
    m_progressBar = new QProgressBar();
    m_progressBar->setRange(0, 0);
    m_progressBar->hide();

    // ----- form
    m_createButton = std::make_shared<QPushButton>("Create");
    connect(m_createButton.get(), &QPushButton::clicked, this,
            &NewBoxPage::onFormSubmit);

    QIcon backIcon = QIcon::fromTheme("go-previous-symbolic");
    m_backButton = std::make_shared<QPushButton>(backIcon, "Back", this);
    m_backButton->setGeometry(10, 10, 80, 40);
    m_backButton->show();

    m_nameEdit = new QLineEdit();
    m_nameEdit->setFont(font);

    m_distroSelect = new QComboBox();
    m_distroSelect->setFont(font);
    for (auto &image : m_images) {
        // TODO icons
        std::string distroName = Distrobox::tryParseDistroFromImageUrl(image);
        if (m_distroIcons->find(distroName) != m_distroIcons->end()) {
            std::string iconPath = m_distroIcons->at(distroName);
            QPixmap pixmap(QString::fromStdString(iconPath));

            m_distroSelect->addItem(pixmap, QString::fromStdString(image));
            m_distroSelect->setIconSize(QSize(24, 24));
        } else {
            m_distroSelect->addItem(QString::fromStdString(image));
        }
    }

    QFormLayout *form = new QFormLayout();
    form->setVerticalSpacing(20);
    form->setHorizontalSpacing(20);
    form->addRow("Name", m_nameEdit);
    form->addRow("Distro Image", m_distroSelect);
    form->addItem(new QSpacerItem(0, 10));
    form->addRow(m_createButton.get());

    QFrame *formFrame = new QFrame();
    formFrame->setFrameShadow(QFrame::Raised);
    formFrame->setFrameShape(QFrame::StyledPanel);
    formFrame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    formFrame->setLayout(form);

    // ------ title
    QLabel *titleLabel = new QLabel("Create A New Box");
    titleLabel->setFont(labelFont);
    titleLabel->setAlignment(Qt::AlignCenter);

    QHBoxLayout *hbox = new QHBoxLayout();
    // hbox->addWidget(m_backButton.get());
    hbox->addStretch(1);
    hbox->addWidget(titleLabel);
    hbox->addStretch(1);

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addItem(hbox);
    vbox->addSpacerItem(new QSpacerItem(0, 20));
    vbox->addWidget(formFrame);
    vbox->addStretch(1);
    vbox->addWidget(m_outputLabel);
    vbox->addWidget(m_progressBar);

    this->setLayout(vbox);

    // async worker
    m_worker = new Worker;
    m_worker->moveToThread(&workerThread);

    connect(&workerThread, &QThread::finished, m_worker, &QObject::deleteLater);
    connect(this, &NewBoxPage::doCreate, m_worker, &Worker::createDistrobox);
    connect(m_worker, &Worker::distroboxCreated, this,
            &NewBoxPage::onBoxCreated);
    workerThread.start();
}

void NewBoxPage::onFormSubmit() {
    m_outputLabel->setText(
        "Creating, please wait...\nThis can take quite a long time");
    m_progressBar->show();
    m_outputLabel->repaint();
    m_progressBar->repaint();

    QString name = m_nameEdit->text();
    QString distro = m_distroSelect->currentText();

    emit doCreate(name, distro);
}

void NewBoxPage::onBoxCreated(QString result) {
    m_outputLabel->setText(result);
    m_progressBar->hide();

    emit newBoxCreated();
}

NewBoxPage::~NewBoxPage() {
    workerThread.quit();
    workerThread.wait();
}
