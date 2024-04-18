///////////////////////////////////////////////////////////////////////////////////////////////////
// dqt_psf.cpp
// 
// Пример диалогового окна для рисования графика и полутонового изображения выборки
// при помощи QCustomPlot (http://www.qcustomplot.com/index.php/introduction).
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "dqt_psf.h"
#include "qcustomplot.h"


DQtPSF::DQtPSF(QWidget* parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);
	setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint);

	// Обработчики
	// Создаем связь между сигналом(событием) от объекта и функцией-обработкой события:
	// connect([кто посылает], &[имя_класса]::[сигнал], [кто обрабатывает], &[имя_класса]::[слот]);
	connect(m_ui.m_pb_get_steps, &QPushButton::clicked, this, &DQtPSF::OnGetStepsClicked);
	connect(m_ui.m_pb_redraw, &QPushButton::clicked, this, &DQtPSF::OnRedrawClicked);
	connect(m_ui.m_pb_cleaned, &QPushButton::clicked, this, &DQtPSF::SetDefaultValues);
	connect(m_ui.m_cb_params, &QComboBox::currentIndexChanged, this, &DQtPSF::OnUnitChanged);
	connect(m_ui.m_pb_calc, &QPushButton::clicked, this, &DQtPSF::OnGetCalc);
	connect(m_ui.m_rb_canon, &QRadioButton::toggled, this, &DQtPSF::OnUnitsChanged);
	connect(m_ui.m_rb_mkm, &QRadioButton::toggled, this, &DQtPSF::OnUnitsChanged);
	connect(m_ui.m_rb_pixels, &QRadioButton::toggled, this, &DQtPSF::OnUnitsChanged);

	// Пусть, пока все значения = 0, кнопка "Сбросить" остается неактивной
	lineEditList = CreateLineEditList();
	for (QLineEdit* lineEdit : lineEditList) {
		connect(lineEdit, &QLineEdit::textChanged, this, &DQtPSF::CheckFieldsForButtDefault);
	}
	// И заодно проверяем вводимые параметры: только числа double
	SetValidatorsForParameters();

	// Покрасим кнопку "Установить значения по умолчанию"
	m_ui.m_pb_cleaned->setStyleSheet("QPushButton:enabled { background-color: #dcc3e8; color: #000000; }"
		"QPushButton:!enabled { background-color: #808080; color: #ffffff; }");

	// Добавим фоновую надпись для выпадающего списка
	m_ui.m_cb_params->setPlaceholderText("Выберите параметр");

	// Устанавливаем значения по умолчанию
	SetDefaultValues();

	// Для отладки: раскомментировать значения "по умолчанию" ниже :)
	//m_ui.m_le_wavelength->setText("0.555");
	//m_ui.m_le_back_aperture->setText("0.5");
	//m_ui.m_le_magnification->setText("1");
	//m_ui.m_le_diam_pupil->setText("8");
	//m_ui.m_cb_params->setCurrentIndex(2);

	// Устанавливаем поля (блокировка)
	OnUnitChanged();

	// Устанавливаем курсор
	m_ui.m_le_wavelength->setFocus();
}


DQtPSF::~DQtPSF()
{
}

void DQtPSF::OnGetCalc() {
	calcPSF.Calc(opt_params, m_PSF);

	is_first_click_psf = true;
	m_ui.m_pb_redraw->setEnabled(is_first_click_psf);
}

void DQtPSF::resizeEvent(QResizeEvent* qEvent_p)
{
	// На случай, если размер виджета не квадратный - задаем одинаковый масштаб по ося X и Y
	RescaleCustomPlot(m_ui.m_qW_Grayscale);
	// Отрисовка
	m_ui.m_qW_Grayscale->replot();
}

void DQtPSF::OnRedrawClicked()
{
	if (!m_PSF.IsZeroSize()) {
		double step = 0;
		// Проверяем, какая опция RadioButton выбрана

		if (m_ui.m_rb_mkm->isChecked()) {
			// Если выбрана опция "m_rb_mkm"
			step = m_ui.m_le_step_im->text().toDouble();
			DrawGraph(m_PSF, m_ui.m_qW_Graph, step, "x, мкм", "Intensity (x)");
			DrawGrayscale(m_PSF, m_ui.m_qW_Grayscale, step, "x, мкм", "y, мкм");
		}
		else if (m_ui.m_rb_pixels->isChecked()) {
			// Если выбрана опция "m_rb_pixels"
			DrawGraph(m_PSF, m_ui.m_qW_Graph, 1, "x, пкс", "Intensity (x)");
			DrawGrayscale(m_PSF, m_ui.m_qW_Grayscale, 1, "x, пкс", "y, пкс");
		}
		else {
			// Если выбрана опция "m_rb_canon" или не выбрано ничего
			m_ui.m_rb_canon->setChecked(true);
			step = m_ui.m_le_step_im_can->text().toDouble();
			DrawGraph(m_PSF, m_ui.m_qW_Graph, step, "x, к. ед.", "Intensity (x)");
			DrawGrayscale(m_PSF, m_ui.m_qW_Grayscale, step, "x, к. ед.", "y, к. ед.");
		};
	};
}

void DQtPSF::DrawGraph(const Sample<double>& PSF_p, QCustomPlot* qGraph_p, double dx_p, QString sXName_p, QString sYName_p)
{
	// Размер выборки
	int dim = PSF_p.GetSizeX();

	// Очищаем предыдущий нарисованный график
	qGraph_p->clearGraphs();
	// Рисуем легенду
	qGraph_p->legend->setVisible(true);

	// Параметры графика сечения по Х
	qGraph_p->addGraph();
	qGraph_p->graph(0)->setPen(QPen(Qt::red));
	qGraph_p->graph(0)->setName("Section X");

	// Параметры графика сечения по Y
	qGraph_p->addGraph();
	qGraph_p->graph(1)->setPen(QPen(Qt::blue));
	qGraph_p->graph(1)->setName("Section Y");

	// Параметры оси Х
	qGraph_p->xAxis->setLabel(sXName_p);
	// Максимальное и минимальное значение по оси
	double min = PSF_p.CalcCoord(0, dx_p);
	double max = PSF_p.CalcCoord(dim, dx_p);
	qGraph_p->xAxis->setRange(min, max);

	// Задаем значения графиков
	min = 0, max = 0;
	for (int i = 0; i < PSF_p.GetSizeX(); i++)
	{
		// Координата точки по Х
		double x = PSF_p.CalcCoord(i, dx_p);

		// Значение в сечении по Х
		double value_x = PSF_p(i, dim / 2);
		qGraph_p->graph(0)->addData(x, value_x);

		// Значение в сечении  по Y
		double value_y = PSF_p(dim / 2, i);
		qGraph_p->graph(1)->addData(x, value_y);

		// Вычисляем min значение по оси X, Y
		if (min > value_x)
			min = value_x;
		if (min > value_y)
			min = value_y;

		// Вычисляем max значение по оси X, Y
		if (max < value_x)
			max = value_x;
		if (max < value_y)
			max = value_y;
	}
	// Параметры оси Y
	qGraph_p->yAxis->setLabel(sYName_p);
	// Максимальное и минимальное значение по оси
	qGraph_p->yAxis->setRange(min, max);
	// Нулевое значение по осям X и Y рисуем толстой линией
	QPen qAxisPen;
	qAxisPen.setWidth(2.);
	qGraph_p->xAxis->grid()->setZeroLinePen(qAxisPen);
	qGraph_p->yAxis->grid()->setZeroLinePen(qAxisPen);

	// Рисуем сами графики
	qGraph_p->replot();
}

void DQtPSF::DrawGrayscale(const Sample<double>& PSF_p, QCustomPlot* qGrayscale_p, double dx_p, QString sXName_p, QString sYName_p)
{
	// Размер выборки
	int dim = PSF_p.GetSize();

	// Очищаем предыдущий нарисованный график
	qGrayscale_p->clearPlottables();

	// Создаем карту уровней для отрисовки и задаем размерность
	QCPColorMap* qMap = new QCPColorMap(qGrayscale_p->xAxis, qGrayscale_p->yAxis);
	qMap->data()->setSize(dim, dim);

	// Задаем максимальное и минимальное значение по осям
	double min = PSF_p.CalcCoord(0, dx_p);
	double max = PSF_p.CalcCoord(dim, dx_p);
	qMap->data()->setRange(QCPRange(min, max), QCPRange(min, max));

	// Заполняем карту уровней значениями из массива sample
	for (int i = 0; i < PSF_p.GetSize(); i++)
	{
		for (int j = 0; j < PSF_p.GetSize(); j++)
		{
			qMap->data()->setCell(i, j, PSF_p(i, j));
		}
	}

	// Устанавливаем тип карты уровней - полутоновое отображение
	qMap->setGradient(QCPColorGradient::gpGrayscale);
	qMap->rescaleDataRange();

	// Параметры оси X
	qGrayscale_p->xAxis->setLabel(sXName_p);
	// Параметры оси Y
	qGrayscale_p->yAxis->setLabel(sYName_p);

	// Пересчитываем оси QCustomPlot под заданную карту уровней
	qGrayscale_p->rescaleAxes();
	// На случай, если размер виджета не квадратный - задаем одинаковый масштаб по осям X и Y
	RescaleCustomPlot(qGrayscale_p);
	// Отрисовка
	qGrayscale_p->replot();
}

void DQtPSF::RescaleCustomPlot(QCustomPlot* qPlot_p)
{
	// Определяем ширину и высоту области для отрисовки
	QSize qRect = qPlot_p->axisRect()->size();
	// Если ширина больше высоты - меняем ось X
	if (qRect.width() > qRect.height())
	{
		qPlot_p->xAxis->setScaleRatio(qPlot_p->yAxis, 1.);
	}
	// Если ширина меньше высоты - меняем ось X
	else if (qRect.width() < qRect.height())
	{
		qPlot_p->yAxis->setScaleRatio(qPlot_p->xAxis, 1.);
	}
}

//-------------------------------------------------------------------------------------------------
// Мой скрипт из лабораторной 4 часть 1 с внесенными правками

void DQtPSF::CheckFieldsForButtDefault()
{
	bool all_fields = true;

	for (QLineEdit* lineEdit : lineEditList) {
		if (lineEdit->text() != "0") {
			all_fields = false;
		}
	}

	m_ui.m_pb_cleaned->setEnabled(!all_fields);
}

void DQtPSF::OnParamChanged()
{
	OnGetSampleSizes();
	OnMultChanged();

	QString selected_param = m_ui.m_cb_params->currentText();

	if (selected_param == "Охват зрачка [к. ед.]") {
		GetValuesFromDiamPupil();

	}
	else if (selected_param == "Шаг по предмету [к. ед.]" || selected_param == "Шаг по изображению [к. ед.]") {
		GetValuesFromStepObIm(selected_param);

	}
	else {
		GetValuesFromStepPupil();
	}
}

void DQtPSF::OnGetSampleSizes()
{
	opt_params.m_int_sample_sizes = m_ui.m_cb_sample_sizes->currentText().toUInt();
}

void DQtPSF::OnMultChanged()
{
	opt_params.m_double_wavelength = m_ui.m_le_wavelength->text().toDouble();
	opt_params.m_double_back_aperture = m_ui.m_le_back_aperture->text().toDouble();
	opt_params.m_double_magnification = m_ui.m_le_magnification->text().toDouble();
	opt_params.m_double_defocus = m_ui.m_le_defocus->text().toDouble();
	opt_params.m_double_astigmatism = m_ui.m_le_astigmatism->text().toDouble();

	opt_params.m_double_aperture = opt_params.m_double_magnification * opt_params.m_double_back_aperture;
}

void DQtPSF::GetValuesFromDiamPupil()
{
	opt_params.m_double_diam_pupil = m_ui.m_le_diam_pupil->text().toDouble();
	opt_params.m_double_step_pupil = opt_params.m_double_diam_pupil / opt_params.m_int_sample_sizes;
	SetCalculateValues();
}

void DQtPSF::GetValuesFromStepObIm(const QString selected_param)
{
	double m_double_step_can = 0;

	if (selected_param == "Шаг по предмету [к. ед.]") {
		m_double_step_can = m_ui.m_le_step_obj_can->text().toDouble();
		m_ui.m_le_step_im_can->setText(QString::number(m_double_step_can));
	}
	else {
		m_double_step_can = m_ui.m_le_step_im_can->text().toDouble();
		m_ui.m_le_step_obj_can->setText(QString::number(m_double_step_can));
	}

	opt_params.m_double_step_pupil = 1 / m_double_step_can / opt_params.m_int_sample_sizes;
	SetCalculateValues();
}

void DQtPSF::GetValuesFromStepPupil()
{
	opt_params.m_double_step_pupil = m_ui.m_le_step_pupil->text().toDouble();
	SetCalculateValues();
}

void DQtPSF::SetCalculateValues() {
	CalculateValues(opt_params.m_double_step_pupil, opt_params.m_int_sample_sizes,
		opt_params.m_double_wavelength, opt_params.m_double_back_aperture,
		opt_params.m_double_aperture);
}

void DQtPSF::CalculateValues(double& step_value, int& sample_sizes, double& wavelength, double& back_aperture, double& aperture)
{
	double diam_pupil = step_value * sample_sizes;
	double step_im_can = 1 / step_value / sample_sizes;
	double step_im = step_im_can * wavelength / back_aperture;
	double step_obj = step_im_can * wavelength / aperture;

	m_ui.m_le_diam_pupil->setText(QString::number(diam_pupil));
	m_ui.m_le_step_im_can->setText(QString::number(step_im_can));
	m_ui.m_le_step_obj_can->setText(QString::number(step_im_can));
	m_ui.m_le_step_im->setText(QString::number(step_im));
	m_ui.m_le_step_obj->setText(QString::number(step_obj));
	m_ui.m_le_step_pupil->setText(QString::number(step_value));

	opt_params.m_int_sample_sizes = m_ui.m_cb_sample_sizes->currentText().toInt();
	opt_params.m_double_wavelength = m_ui.m_le_wavelength->text().toDouble();
	opt_params.m_double_back_aperture = m_ui.m_le_back_aperture->text().toDouble();
	opt_params.m_double_magnification = m_ui.m_le_magnification->text().toDouble();
	opt_params.m_double_defocus = m_ui.m_le_defocus->text().toDouble();
	opt_params.m_double_astigmatism = m_ui.m_le_astigmatism->text().toDouble();
	opt_params.m_double_step_obj = m_ui.m_le_step_obj->text().toDouble();
	opt_params.m_double_step_obj_can = m_ui.m_le_step_obj_can->text().toDouble();
	opt_params.m_double_step_im = m_ui.m_le_step_im->text().toDouble();
	opt_params.m_double_step_im_can = m_ui.m_le_step_im_can->text().toDouble();
	opt_params.m_double_diam_pupil = m_ui.m_le_diam_pupil->text().toDouble();
	opt_params.m_double_step_pupil = m_ui.m_le_step_pupil->text().toDouble();
}

void DQtPSF::OnUnitChanged()
{
	m_ui.m_le_step_obj_can->setEnabled(false);
	m_ui.m_le_step_obj->setEnabled(false);
	m_ui.m_le_step_im_can->setEnabled(false);
	m_ui.m_le_step_im->setEnabled(false);
	m_ui.m_le_diam_pupil->setEnabled(false);
	m_ui.m_le_step_pupil->setEnabled(false);

	QString selected_param = m_ui.m_cb_params->currentText();

	if (selected_param == "Охват зрачка [к. ед.]") {
		m_ui.m_le_diam_pupil->setEnabled(true);
	}
	else if (selected_param == "Шаг по предмету [к. ед.]") {
		m_ui.m_le_step_obj_can->setEnabled(true);
	}
	else if (selected_param == "Шаг по изображению [к. ед.]") {
		m_ui.m_le_step_im_can->setEnabled(true);
	}
	else if (selected_param == "Шаг по зрачку [к. ед.]") {
		m_ui.m_le_step_pupil->setEnabled(true);
	}
}

void DQtPSF::SetDoubleValidator(QLineEdit* lineEdit)
{
	QLocale locale = QLocale::c();
	locale.setNumberOptions(QLocale::RejectGroupSeparator);
	QDoubleValidator* validator = new QDoubleValidator(lineEdit);
	validator->setLocale(locale);
	validator->setBottom(0.0);
	lineEdit->setValidator(validator);
}

QList<QLineEdit*> DQtPSF::CreateLineEditList()
{
	QList<QLineEdit*> lineEditList = {
		m_ui.m_le_wavelength,
		m_ui.m_le_back_aperture,
		m_ui.m_le_magnification,
		m_ui.m_le_defocus,
		m_ui.m_le_astigmatism,
		m_ui.m_le_step_obj,
		m_ui.m_le_step_obj_can,
		m_ui.m_le_step_im,
		m_ui.m_le_step_im_can,
		m_ui.m_le_diam_pupil,
		m_ui.m_le_step_pupil
	};

	return lineEditList;
}

void DQtPSF::SetValidatorsForParameters()
{
	for (QLineEdit* lineEdit : lineEditList) {
		SetDoubleValidator(lineEdit);
	}
}

void DQtPSF::SetDefaultValues()
{
	// Для установки значений по умолчанию (0) расскомментировать ниже:
	for (QLineEdit* lineEdit : lineEditList) {
		lineEdit->setText("0");
	}

	m_ui.m_cb_sample_sizes->setCurrentIndex(0);

	m_ui.m_cb_params->setCurrentIndex(-1);

	if (is_first_click_psf) {
		m_ui.m_qW_Grayscale->clearPlottables();
		m_ui.m_qW_Graph->clearGraphs();
		m_ui.m_qW_Graph->legend->setVisible(false);
		m_ui.m_qW_Grayscale->xAxis->setLabel("");
		m_ui.m_qW_Grayscale->yAxis->setLabel("");
		m_ui.m_qW_Graph->xAxis->setLabel("");
		m_ui.m_qW_Graph->yAxis->setLabel("");

		m_ui.m_qW_Graph->xAxis->grid()->setZeroLinePen(Qt::NoPen);
		m_ui.m_qW_Graph->yAxis->grid()->setZeroLinePen(Qt::NoPen);

		m_ui.m_qW_Grayscale->replot();
		m_ui.m_qW_Graph->replot();
	};

	is_first_click_psf = false;
	is_first_click_steps = false;
	m_ui.m_pb_calc->setEnabled(is_first_click_steps);
	m_ui.m_pb_redraw->setEnabled(is_first_click_psf);

	m_ui.m_pb_get_steps->setText("Посчитать шаги");

	// Отключаем выделение у всех RadioButton
	m_ui.m_rb_canon->setAutoExclusive(false);
	m_ui.m_rb_mkm->setAutoExclusive(false);
	m_ui.m_rb_pixels->setAutoExclusive(false);

	// Сбрасываем выбор 
	m_ui.m_rb_canon->setChecked(false);
	m_ui.m_rb_mkm->setChecked(false);
	m_ui.m_rb_pixels->setChecked(false);
}

void DQtPSF::OnGetStepsClicked()
{
	bool err = false;
	QString first_en_par;

	if (m_ui.m_cb_params->currentIndex() == -1) {
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
	}
	else {
		OnParamChanged();
		if (!is_first_click_steps) {
			m_ui.m_pb_get_steps->setText("Пересчитать шаги");
			is_first_click_steps = true;
		}
	}

	if (is_first_click_steps) {
		m_ui.m_pb_calc->setEnabled(true);
	}
}

void DQtPSF::OnUnitsChanged()
{
	m_ui.m_rb_canon->setAutoExclusive(true);
	m_ui.m_rb_mkm->setAutoExclusive(true);
	m_ui.m_rb_pixels->setAutoExclusive(true);
}
