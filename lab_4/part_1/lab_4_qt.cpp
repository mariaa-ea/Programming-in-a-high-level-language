#include "lab_4_qt.h"
#include "ui_lab_4_qt.h"

#include <QMessageBox>
#include <QDoubleValidator>

lab_4_qt::lab_4_qt(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::lab_4_qt)
{
    // Соединяет форму с нашим классом, только после этой строки пишется остальная реализация
    ui->setupUi(this);

    // Обработчики
    // connect([кто посылает], &[имя_класса]::[сигнал], [кто обрабатывает], &[имя_класса]::[слот]);
    connect(ui->m_pb_get_steps, &QPushButton::clicked, this, &lab_4_qt::OnGetStepsClicked);
    connect(ui->m_pb_redraw, &QPushButton::clicked, this, &lab_4_qt::OnRedrawClicked);
    connect(ui->m_pb_cleaned, &QPushButton::clicked, this, &lab_4_qt::SetDefaultValues);
    connect(ui->m_cb_params, &QComboBox::currentIndexChanged, this, &lab_4_qt::OnUnitChanged);

    // Пусть, пока все значения = 0, кнопка "Установить значения по умолчанию" остается неактивной
    lineEditList = CreateLineEditList();
    for (QLineEdit* lineEdit : lineEditList) {
        connect(lineEdit, &QLineEdit::textChanged, this, &lab_4_qt::CheckFieldsForButtDefault);
    }
    // И заодно проверяем вводимые параметры: только числа double
    SetValidatorsForParameters();

    // Покрасим кнопку "Установить значения по умолчанию"
    ui->m_pb_cleaned->setStyleSheet("QPushButton:enabled { background-color: #dcc3e8; color: #000000; }"
                                    "QPushButton:!enabled { background-color: #808080; color: #ffffff; }");

    // Добавим фоновую надпись для выпадающего списка
    ui->m_cb_params->setPlaceholderText("Выберите параметр");

    // Устанавливаем значения по умолчанию
    SetDefaultValues();

    // Устанавливаем поля (блокировка)
    OnUnitChanged();
}

lab_4_qt::~lab_4_qt()
{
    delete ui;
}

void lab_4_qt::CheckFieldsForButtDefault()
{
    bool all_fields = true;

    for (QLineEdit* lineEdit : lineEditList) {
        if (lineEdit->text() != "0") {
            all_fields = false;
        }
    }

    ui->m_pb_cleaned->setEnabled(!all_fields);
}

void lab_4_qt::OnParamChanged()
{
    OnGetSampleSizes();
    OnMultChanged();

    QString selected_param = ui->m_cb_params->currentText();

    if (selected_param == "Охват зрачка [к. ед.]") {
        GetValuesFromDiamPupil();

    } else if (selected_param == "Шаг по предмету [к. ед.]" || selected_param == "Шаг по изображению [к. ед.]") {
        GetValuesFromStepObIm(selected_param);

    } else {
        GetValuesFromStepPupil();
    }
}

void lab_4_qt::OnGetSampleSizes()
{
    m_int_sample_sizes = ui->m_cb_sample_sizes->currentText().toUInt();
}

void lab_4_qt::OnMultChanged()
{
    opt_params.m_double_wavelength = ui->m_le_wavelength ->text().toDouble();
    opt_params.m_double_back_aperture = ui->m_le_back_aperture ->text().toDouble();
    opt_params.m_double_magnification = ui->m_le_magnification ->text().toDouble();
    opt_params.m_double_defocus = ui->m_le_defocus ->text().toDouble();
    opt_params.m_double_astigmatism = ui->m_le_astigmatism ->text().toDouble();

    opt_params.m_double_aperture = opt_params.m_double_magnification * opt_params.m_double_back_aperture;
}

void lab_4_qt::GetValuesFromDiamPupil()
{
    opt_params.m_double_diam_pupil = ui->m_le_diam_pupil->text().toDouble();
    opt_params.m_double_step_pupil = opt_params.m_double_diam_pupil / m_int_sample_sizes;
    SetCalculateValues();
}

void lab_4_qt::GetValuesFromStepObIm(const QString selected_param)
{
    double m_double_step_can = 0;

    if (selected_param == "Шаг по предмету [к. ед.]") {
        m_double_step_can = ui->m_le_step_obj_can->text().toDouble();
        ui->m_le_step_im_can->setText(QString::number(m_double_step_can));
    } else {
        m_double_step_can = ui->m_le_step_im_can->text().toDouble();
        ui->m_le_step_obj_can->setText(QString::number(m_double_step_can));
    }

    opt_params.m_double_step_pupil = 1 / m_double_step_can / m_int_sample_sizes;
    SetCalculateValues();
}

void lab_4_qt::GetValuesFromStepPupil()
{
    opt_params.m_double_step_pupil = ui->m_le_step_pupil->text().toDouble();
    SetCalculateValues();
}

void lab_4_qt::SetCalculateValues() {
    CalculateValues(opt_params.m_double_step_pupil, m_int_sample_sizes,
                    opt_params.m_double_wavelength, opt_params.m_double_back_aperture,
                    opt_params.m_double_aperture);
}

void lab_4_qt::CalculateValues(double& step_value, int& sample_sizes, double& wavelength, double& back_aperture, double& aperture)
{
    double diam_pupil = step_value * sample_sizes;
    double step_im_can = 1 / step_value / sample_sizes;
    double step_im = step_im_can * wavelength / back_aperture;
    double step_obj = step_im_can * wavelength / aperture;

    ui->m_le_diam_pupil->setText(QString::number(diam_pupil));
    ui->m_le_step_im_can->setText(QString::number(step_im_can));
    ui->m_le_step_obj_can->setText(QString::number(step_im_can));
    ui->m_le_step_im->setText(QString::number(step_im));
    ui->m_le_step_obj->setText(QString::number(step_obj));
    ui->m_le_step_pupil->setText(QString::number(step_value));
}

void lab_4_qt::OnUnitChanged()
{
        ui->m_le_step_obj_can->setEnabled(false);
        ui->m_le_step_obj->setEnabled(false);
        ui->m_le_step_im_can->setEnabled(false);
        ui->m_le_step_im->setEnabled(false);
        ui->m_le_diam_pupil->setEnabled(false);
        ui->m_le_step_pupil->setEnabled(false);

        QString selected_param = ui->m_cb_params->currentText();

        if (selected_param == "Охват зрачка [к. ед.]") {
            ui->m_le_diam_pupil->setEnabled(true);
        } else if (selected_param == "Шаг по предмету [к. ед.]") {
            ui->m_le_step_obj_can->setEnabled(true);
        } else if (selected_param == "Шаг по изображению [к. ед.]") {
            ui->m_le_step_im_can->setEnabled(true);
        } else if (selected_param == "Шаг по зрачку [к. ед.]") {
            ui->m_le_step_pupil->setEnabled(true);
        }
}

void lab_4_qt::SetDoubleValidator(QLineEdit* lineEdit)
{
    QLocale locale = QLocale::c();
    locale.setNumberOptions(QLocale::RejectGroupSeparator);
    QDoubleValidator *validator = new QDoubleValidator(lineEdit);
    validator->setLocale(locale);
    validator->setBottom(0.0);
    lineEdit->setValidator(validator);
}

QList<QLineEdit*> lab_4_qt::CreateLineEditList()
{
    QList<QLineEdit*> lineEditList = {
        ui->m_le_wavelength,
        ui->m_le_back_aperture,
        ui->m_le_magnification,
        ui->m_le_defocus,
        ui->m_le_astigmatism,
        ui->m_le_step_obj,
        ui->m_le_step_obj_can,
        ui->m_le_step_im,
        ui->m_le_step_im_can,
        ui->m_le_diam_pupil,
        ui->m_le_step_pupil
    };

    return lineEditList;
}

void lab_4_qt::SetValidatorsForParameters()
{
    for (QLineEdit* lineEdit : lineEditList) {
        SetDoubleValidator(lineEdit);
    }
}

void lab_4_qt::SetDefaultValues()
{
    for (QLineEdit* lineEdit : lineEditList) {
        lineEdit->setText("0");
    }

    ui->m_cb_sample_sizes->setCurrentIndex(0);

    ui->m_cb_params->setCurrentIndex(-1);

    is_first_click = true;
    ui->m_pb_get_steps->setText("Посчитать шаги");
}

void lab_4_qt::OnGetStepsClicked()
{
    bool err = false;
    QString first_en_par;

    if (ui->m_cb_params->currentIndex() == -1) {
        QMessageBox::critical(this, "Ошибка", "Выберите параметр для расчета!");
        return;
    }

    for (QLineEdit* lineEdit : lineEditList) {
        if (lineEdit->isEnabled() && lineEdit->text().isEmpty()) {
            QString field_name = lineEdit->objectName();
            if (!err) {
                first_en_par = field_name;
            }
            err = true;
        }
    }

    if (err) {
        QMessageBox::critical(this, "Ошибка", "Поле '" + first_en_par + "' не установлено!");
    } else {
        OnParamChanged();
        if (is_first_click) {
            ui->m_pb_get_steps->setText("Пересчитать шаги");
            is_first_click = false;
        }
    }
}

void lab_4_qt::OnRedrawClicked()
{
    // Реализация будет позже
}
