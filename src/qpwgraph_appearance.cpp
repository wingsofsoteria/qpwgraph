#include "config.h"
#include <QFontDatabase>
#include "qpwgraph_appearance.h"

#include "qpwgraph_main.h"
#include "qpwgraph_config.h"

#include <QMessageBox>
#include <QPushButton>

#include <QLineEdit>
#include <qapplication.h>
#include <qfont.h>
#include <qfontdatabase.h>
#include <qlist.h>
#include <qlistwidget.h>
#include <qnamespace.h>
#include <qobject.h>
#include <QListWidgetItem>
#include <qtypes.h>

qpwgraph_appearance::qpwgraph_appearance (qpwgraph_main *parent)
    : QDialog(parent)
{
    m_ui.setupUi(this);
    m_dirty = 0;
	QStringList families = QFontDatabase::families();

    m_ui.FontList->addItems(QFontDatabase::families());

	qpwgraph_config *config = parent->config();
	if (config) {
		QList<QListWidgetItem*> items = m_ui.FontList->findItems(config->fontFamilyName(), Qt::MatchFlag::MatchFixedString);
		m_ui.FontList->setCurrentItem(items[0]);
		m_ui.FontSizeSlider->setValue(config->fontPointSize());
		m_ui.FontSizeBox->setText(QString::number(config->fontPointSize()));
		m_selected_font = QFont(config->fontFamilyName(), m_ui.FontSizeSlider->value());
	}
    
    QObject::connect(m_ui.DialogButtonBox,
		SIGNAL(accepted()),
		SLOT(accept()));
	QObject::connect(m_ui.DialogButtonBox,
		SIGNAL(rejected()),
		SLOT(reject()));
	QObject::connect(m_ui.FontSearchBox,
		SIGNAL(editingFinished()),
		SLOT(searchFont()));
    QObject::connect(m_ui.FontList,
        SIGNAL(itemClicked(QListWidgetItem *)),
        SLOT(updateSelectedFont()));
    QObject::connect(m_ui.FontSizeSlider,
        SIGNAL(valueChanged(int)),
        SLOT(updateSelectedFont()));
    stabilize();
}

qpwgraph_appearance::~qpwgraph_appearance (void)
{

}
void qpwgraph_appearance::updateSelectedFont()
{
    
    QString font_family = m_ui.FontList->currentItem()->text();
	int font_size = m_ui.FontSizeSlider->value();
    m_ui.FontDisplayTextBox->setFont(QFont(font_family, font_size));
    m_ui.FontSizeBox->setText(QString::number(font_size));
	m_selected_font.setFamily(font_family);
	m_selected_font.setPointSize(font_size);
	changed();
}

// Reject options (Cancel button slot).
void qpwgraph_appearance::reject (void)
{
	bool ret = true;

	// Check if there's any pending changes...
	if (m_dirty > 0) {
		switch (QMessageBox::warning(this,
			tr("Warning"),
			tr("Some options have been changed.") + "\n\n" +
			tr("Do you want to apply the changes?"),
			QMessageBox::Apply | QMessageBox::Discard | QMessageBox::Cancel)) {
		case QMessageBox::Apply:
			accept();
			return;
		case QMessageBox::Discard:
			break;
		default: // Cancel.
			ret = false;
		}
	}

	if (ret)
		QDialog::reject();
}

void qpwgraph_appearance::accept (void)
{
	qpwgraph_config *config = nullptr;
	qpwgraph_main *parent = qobject_cast<qpwgraph_main *> (parentWidget());
	if (parent)
		config = parent->config();
	if (config) {
		config->setFontFamilyName(m_selected_font.family());
		config->setFontPointSize(m_selected_font.pointSize());
		config->setFontWeight(m_selected_font.weight());
		config->setFontIsItalic(m_selected_font.italic());
	}
	QApplication::setFont(m_selected_font);
    QDialog::accept();
}

void qpwgraph_appearance::changed (void)
{
	++m_dirty;

	stabilize();
}

void qpwgraph_appearance::searchFont() {
	QString partialFontName = m_ui.FontSearchBox->text();
	QList<QListWidgetItem*> matchingItems = m_ui.FontList->findItems(partialFontName, Qt::MatchFlag::MatchContains);
	for (int i = 0; i < m_ui.FontList->count(); i++) {
		QListWidgetItem* currentItem = m_ui.FontList->item(i);
		if (matchingItems.contains(currentItem)) {
			m_ui.FontList->setRowHidden(i, true);
		}
	}

}

void qpwgraph_appearance::stabilize (void)
{
	m_ui.DialogButtonBox->button(QDialogButtonBox::Ok)->setEnabled(m_dirty > 0);
}