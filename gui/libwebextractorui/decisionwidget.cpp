#include "decisionwidget.h"
#include "changelogwidget.h"
#include "ui_decisionForm.h"

namespace NS = Nepomuk::Sync;
namespace NW = Nepomuk::WebExtractor;

class DecisionWidget::Private
{
    public:
	Ui_DecisionForm * ui;
	NW::Decision decision;
	ChangeLogWidget * logWidget;
};

void DecisionWidget::setupUi()
{
    d->ui = new Ui_DecisionForm();
    d->ui->setupUi(this);
    d->logWidget = new ChangeLogWidget(this);
    Q_ASSERT(this->layout());
    this->layout()->addWidget(d->logWidget);
}

DecisionWidget::DecisionWidget(QWidget * parent):
    QWidget(parent),
    d( new Private() )
{
    setupUi();
}

DecisionWidget::DecisionWidget( const NW::Decision & decision,QWidget * parent):
    QWidget(parent),
    d( new Private() )
{
    setupUi();
    setDecision(decision);
}

DecisionWidget::~DecisionWidget()
{
    delete d->ui;
    delete d;
}

void DecisionWidget::setDecision( const NW::Decision & decision)
{
    d->decision = decision;
    d->logWidget->setLog(decision.log());
    d->ui->descriptionBrowser->setPlainText(decision.description());
}
void DecisionWidget::clear()
{
    d->logWidget->clear();
    d->decision = NW::Decision();
}
