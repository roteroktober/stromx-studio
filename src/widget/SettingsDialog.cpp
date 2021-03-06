#include "widget/SettingsDialog.h"

#include <QGroupBox>
#include <QLabel>
#include <QSpinBox>
#include <QVBoxLayout>
#include "model/StreamModel.h"

SettingsDialog::SettingsDialog(QWidget* parent)
  : QDialog(parent),
    m_delayDurationSpinBox(0),
    m_accessTimeoutSpinBox(0),
    m_stream(0)
{
    QGroupBox* box = 0;
    QLabel* label = 0;
    QLabel* documentation = 0;
    QVBoxLayout* groupBoxLayout = 0;
    QHBoxLayout* editLayout = 0;
    
    
    QVBoxLayout* dialogLayout = new QVBoxLayout;
    
    box = new QGroupBox(tr("Stream access"));
    label = new QLabel(tr("Time out (ms)"));
    documentation = new QLabel(tr("\
Operations which read or write a parameter of an operator \
can block some time if the stream is active and under heavy load. \
However, if either the stream design or the one of the operators is \
faulty, the operation can be blocked forever.\n\
When accessing a parameter stroms-studio waits a pre-defined amount of time \
after which it decides that the latter is the case. This time is defined here."));
    documentation->setWordWrap(true);
    m_accessTimeoutSpinBox = new QSpinBox();
    m_accessTimeoutSpinBox->setMinimum(0);
    m_accessTimeoutSpinBox->setMaximum(1e6);
    m_accessTimeoutSpinBox->setSingleStep(100);
    
    groupBoxLayout = new QVBoxLayout;
    editLayout = new QHBoxLayout;
    editLayout->addWidget(label);
    editLayout->addWidget(m_accessTimeoutSpinBox);
    groupBoxLayout->addItem(editLayout);
    groupBoxLayout->addWidget(documentation);
    box->setLayout(groupBoxLayout);
    dialogLayout->addWidget(box);
    
    box = new QGroupBox(tr("Slow processing"));
    label = new QLabel(tr("Delay (ms)"));
    documentation = new QLabel(tr("\
Slow processing refers to a mode where the stream delays the execution for a \
pre-defined amount of time each time it sends data to an operator input. \
This improves the visualization of the data for streams which process data \
very fast. The length of the delay is defined here."));
    documentation->setWordWrap(true);
    m_delayDurationSpinBox = new QSpinBox();
    m_delayDurationSpinBox->setMinimum(1);
    m_delayDurationSpinBox->setMaximum(1e6);
    m_delayDurationSpinBox->setSingleStep(100);
    
    groupBoxLayout = new QVBoxLayout;
    editLayout = new QHBoxLayout;
    editLayout->addWidget(label);
    editLayout->addWidget(m_delayDurationSpinBox);
    groupBoxLayout->addItem(editLayout);
    groupBoxLayout->addWidget(documentation);
    box->setLayout(groupBoxLayout);
    dialogLayout->addWidget(box);
    
    setLayout(dialogLayout);
}

void SettingsDialog::setModel(StreamModel* stream)
{
    Q_ASSERT(stream);
    
    // remember the stream
    m_stream = stream;
    
    // get the current values from the stream
    m_delayDurationSpinBox->setValue(stream->delayDuration());
    m_accessTimeoutSpinBox->setValue(stream->accessTimeout());
    
    // synchronize the dialog with the stream settings
    connect(stream, SIGNAL(delayDurationChanged(int)), m_delayDurationSpinBox, SLOT(setValue(int)));
    connect(stream, SIGNAL(accessTimeoutChanged(int)), m_accessTimeoutSpinBox, SLOT(setValue(int)));
    connect(m_delayDurationSpinBox, SIGNAL(editingFinished()), this, SLOT(setDelayDuration()));
    connect(m_accessTimeoutSpinBox, SIGNAL(editingFinished()), this, SLOT(setAccessTimeout()));
}

void SettingsDialog::setAccessTimeout()
{
    Q_ASSERT(m_stream);
    m_stream->setAccessTimeout(m_accessTimeoutSpinBox->value());
}

void SettingsDialog::setDelayDuration()
{
    Q_ASSERT(m_stream);
    m_stream->setDelayDuration(m_delayDurationSpinBox->value());
}



