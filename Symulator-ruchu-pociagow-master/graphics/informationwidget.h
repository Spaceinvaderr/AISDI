#ifndef INFORMATIONWIDGET_H
#define INFORMATIONWIDGET_H

#include <QtGui>
#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QVBoxLayout>
#include "../engine/engine.hpp"

class InformationWidget : public QWidget
{
	Q_OBJECT
public:
	InformationWidget(Engine *, QWidget *parent = 0);
	void clear();
	void init();
	~InformationWidget();
protected:

public slots:
	void update();
signals:
	void activeChangedSig(const Engine::TrainState *);
private slots:
	void activeChanged(int);
private:
	void createWidgets();
	Engine *engine;
	int lastActive;
	QLabel *trainLabel;
	QComboBox *trainsList;
	QLabel *trainInfoLabel;
	QVBoxLayout *layout;

};

#endif // INFORMATIONWIDGET_H
