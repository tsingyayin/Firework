#pragma once
#include <QtCore>
#include <QtWidgets>

namespace CE {
	class CEInfoPage :public QFrame
	{
	public:
		QLabel* InfoLabel;
		QPushButton* YesButton;
		CEInfoPage() {
			this->setMinimumSize(700, 300);
			this->setWindowModality(Qt::WindowModality::ApplicationModal);
			InfoLabel = new QLabel(this);
			YesButton = new QPushButton(this);
			YesButton->setText("È·¶¨");
			connect(YesButton, SIGNAL(clicked()), this, SLOT(close()));
			InfoLabel->setWordWrap(TRUE);
			InfoLabel->setAlignment(Qt::AlignCenter);
			this->setAttribute(Qt::WA_DeleteOnClose);
		}
		void setText(QString text) {
			InfoLabel->setText(text);
		}
		void setButtonText(QString text) {
			YesButton->setText(text);
		}
		void setWordWarp(bool warp) {
			InfoLabel->setWordWrap(warp);
		}
		void setAlignment(Qt::AlignmentFlag Alignment) {
			InfoLabel->setAlignment(Alignment);
		}
		void resizeEvent(QResizeEvent* event = Q_NULLPTR) {
			InfoLabel->setGeometry(0, 0, width(), height() * 0.85);
			YesButton->setGeometry(width() * 0.45, height() * 0.855, width() * 0.1, height() * 0.1);
		}
	};
}