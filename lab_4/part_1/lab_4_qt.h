#ifndef LAB_4_QT_H
#define LAB_4_QT_H

#include <QMainWindow>
#include <QLineEdit>

struct OpticalParameters {
    // Параметры оптической системы
    double m_double_wavelength = 0;
    double m_double_back_aperture = 0;
    double m_double_magnification = 0;
    double m_double_defocus = 0;
    double m_double_astigmatism = 0;

    // Численные параметры
    double m_double_step_obj = 0;
    double m_double_step_obj_can = 0;
    double m_double_step_im = 0;
    double m_double_step_im_can = 0;
    double m_double_diam_pupil = 0;
    double m_double_step_pupil = 0;

    double m_double_aperture = 0;
};

QT_BEGIN_NAMESPACE
namespace Ui {
class lab_4_qt;
}
QT_END_NAMESPACE

class lab_4_qt : public QMainWindow
{
    Q_OBJECT

public:
    lab_4_qt(QWidget *parent = nullptr);
    ~lab_4_qt();


private slots:
    // Считывает значения с LineEdit-полей
    void OnMultChanged();

    // Проверяет наличие всех введенных параметров и осуществляет их пересчет
    void OnGetStepsClicked();

    // Возвращает выбранное пользователем значение выборки
    void OnGetSampleSizes();

    // Используется, чтобы пользователь мог ставить только точку в числах типа double
    void SetDoubleValidator(QLineEdit* lineEdit);
    // Устанавливает проверку на все вводимые пользователем параметры
    void SetValidatorsForParameters();

    // Кнопка выпадающего списка: в зависимости от выбора пользователя
    // одно из окон становится активным
    void OnUnitChanged();

    // Считает значения, если задан охват зрачка
    void GetValuesFromDiamPupil();
    // Считает значения, если задан шаг предмета или изображения
    void GetValuesFromStepObIm(const QString selected_param);
    // Считает значения, если задан шаг зрачка
    void GetValuesFromStepPupil();
    // Устанавливает значения в полях
    void CalculateValues(double& step_value, int& sample_sizes, double& wavelength, double& back_aperture, double& aperture);
    // Осуществляет вызов CalculateValues с заданными параметрами
    void SetCalculateValues();

    // Производит расчет параметров в зависимости от выбора пользователя
    void OnParamChanged();

    // Проверяет: есть ли отличные от 0 значения или нет (чтобы определить активность кнопки "Установить значения по умолчанию")
    void CheckFieldsForButtDefault();

    // Устанавливает значения по умолчанию
    void SetDefaultValues();

    // Рисует график
    void OnRedrawClicked();

    // Создает объект, в котором хранятся все переменные QLineEdit
    QList<QLineEdit*> CreateLineEditList();

private:
    Ui::lab_4_qt *ui;

    // Размер выборки
    int m_int_sample_sizes = 0;

    OpticalParameters opt_params;

    // Список имен всех LineEdit-полей
    QList<QLineEdit*> lineEditList;

    // Хранит состояние кнопки "Посчитать шаги"
    bool is_first_click = true;

};
#endif // LAB_4_QT_H
