/**
 ******************************************************************************
 *
 * @file       i2c_vmwidget.cpp
 * @author     Tau Labs, http://www.taulabs.org, Copyright (C) 2013
 * @addtogroup GCSPlugins GCS Plugins
 * @{
 * @addtogroup I2C_VMPlugin
 * @{
 * @brief
 *****************************************************************************/
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */
#include "i2c_vmwidget.h"
#include "i2c_vmfactory.h"
#include "i2c_vm.h"

#include <QDebug>
#include <QtGui/QWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QPushButton>
#include <QScrollArea>
//#include "uavobjectwidgetutils/configtaskwidget.h"

using namespace std;

I2C_VMWidget::I2C_VMWidget(QWidget *parent) : QLabel(parent)
{


    m_widget = new Ui_I2C_VMWidget();
    m_widget->setupUi(this);

//    addUAVObject("I2CVM");

//    addApplySaveButtons(m_widget->ApplyPushButton,m_widget->SavePushButton);

    m_widget->hexDecComboBox->addItem("Hexadecimal");
    m_widget->hexDecComboBox->addItem("Decimal");

//    instructionTypes << "None" << "Write" << "Read" << "Delay [ms]" << "Send UAVO";
//    m_widget->InstructionComboBox_0->addItems(instructionTypes);
//    m_widget->InstructionComboBox_0->setCurrentIndex(0);

    //Connect elements to callback functions
//    connect(m_widget->InstructionComboBox_0, SIGNAL(currentIndexChanged(QString)), this, SLOT(switchCompilerInst(QString)));
    connect(m_widget->AdditionalLinesPushButton, SIGNAL(clicked()), this, SLOT(addAdditionalCompilerLine()));
    connect(m_widget->ApplyPushButton, SIGNAL(clicked()), this, SLOT(applyCompiler()));
    connect(m_widget->hexDecComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(setHexDecRepresentation(QString)));

    //Add first programming line
    instrctIdx=0; // <--- *must* come beofre addAdditionalCompilerLine()
    addAdditionalCompilerLine();


//    setAttribute(Qt::WA_LayoutUsesWidgetRect);

    //Add scroll bar when necessary
//    QScrollArea *scroll = new QScrollArea;
//    scroll->setWidget(m_widget->frame);
//    m_widget->gridLayout->addWidget(scroll);


}

I2C_VMWidget::~I2C_VMWidget()
{
    // Do nothing
}

void I2C_VMWidget::addAdditionalCompilerLine(){
    qDebug()<<"Add compiler line";
    VMInstructionForm *form = new VMInstructionForm(instrctIdx, this);

    //Add form to widget
    m_widget->vmInstructionLayout->addWidget(form);

    //Set hexadecimal representation
    if(m_widget->hexDecComboBox->currentText()=="Hexadecimal")
        form->setHexRepresentation(true);
    else
        form->setHexRepresentation(false);

    //Add form to array for future use
    formList.push_back(form);
    for (int i=0; i< formList.size(); i++){
        formList[i]->setNumInstructions(formList.size());
    }

    instrctIdx++;
}

void I2C_VMWidget::applyCompiler(){
    qDebug()<<"Compiling...";
    if (m_widget->hexDecComboBox->currentText()=="Decimal")
        generateVmCode(m_widget->I2CAddressLabel->text().toInt(0,10), formList);
    else
        generateVmCode(m_widget->I2CAddressLabel->text().toInt(0,16), formList);
}

void I2C_VMWidget::setHexDecRepresentation(QString){
    for (int i=0; i< formList.size(); i++){
        if(m_widget->hexDecComboBox->currentText()=="Hexadecimal")
            formList[i]->setHexRepresentation(true);
        else
            formList[i]->setHexRepresentation(false);
    }
}
