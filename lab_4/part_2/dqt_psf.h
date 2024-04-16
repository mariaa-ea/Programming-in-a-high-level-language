///////////////////////////////////////////////////////////////////////////////////////////////////
// dqt_psf.h
// 
// Диалоговое окно для отрисовки графика и полутонового изображения выборки
// при помощи QCustomPlot (http://www.qcustomplot.com/index.php/introduction).
///////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QDialog>

#include "ui_dqt_psf.h"
#include "calc_psf.h"


constexpr double PI = 3.14159265358979;


class QCustomPlot;

class DQtPSF : public QDialog
{
	Q_OBJECT

public:
	// конструктор
	DQtPSF(QWidget* parent = 0);
	// деструктор
	~DQtPSF();

	CalcPSF calcPSF;


	// обработка событий
public slots:

	// Перехватывает событие Resize для диалогового окна
	// (это нужно, чтобы сохранить одинаковые масштабы по осям Х и Y для полутонового отображения)
	void resizeEvent(QResizeEvent* qEvent_p);

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

	// Вычисляет значение ФРТ
	void OnGetCalc();

	// Создает объект, в котором хранятся все переменные QLineEdit
	QList<QLineEdit*> CreateLineEditList();

	// Определяет состояние кнопок - единиц измерения - для отрисовки графиков
	void OnUnitsChanged();

private:

	OpticalParameters opt_params;

	// Список имен всех LineEdit-полей
	QList<QLineEdit*> lineEditList;

	// Хранит состояние кнопки "Посчитать шаги"
	bool is_first_click_steps = false;
	// Хранит состояние кнопки "Вычислить ФРТ"
	bool is_first_click_psf = false;

private:
	// Рисует график центральных сечений по Х и Y для двумерного массива
	// sample - двумерный массив
	// qGraph - имя виджета, на котором рисовать
	// параметры графика:
	// dx - шаг по оси Х
	// sXName, sYName - подписи к осям Х, Y
	void DrawGraph(const Sample<double>& oSample_p, QCustomPlot* qGraph_p, double dx_p, QString sXName_p, QString sYName_p);

	// Рисует полутоновое отображение двумерного массива
	// sample - двумерный массив
	// qGraph - имя виджета, на котором рисовать
	// параметры графика:
	// dx - шаг по оси Х
	// sXName, sYName - подписи к осям Х, Y
	void DrawGrayscale(const Sample<double>& oSample_p, QCustomPlot* qGrayscale_p, double dx_p, QString sXName_p, QString sYName_p);

	// Функция, задающая одинаковый масштаб по осям X и Y
	// должна вызываться при первоначальной отрисовке и при масштабировании
	void RescaleCustomPlot(QCustomPlot* qPlot_p);


	// ресурсы диалога
	Ui::DQtPSF m_ui;
	Sample<double> m_PSF;
};
