#ifndef __SCOREEDITORDLG__
#define __SCOREEDITORDLG__

#include <map>
#include <vector>
#include <string>
#include "SMSTransformationChainConfig.hxx"
#include "ScoreEditorDlgBase.h"

class QBoxLayout;

namespace QtSMS
{
	class SMSConfigurator;
	
	class ScoreEditorDlg : public ScoreEditorDlgBase
	{
		typedef std::map<std::string,QWidget*> HelpWidgetTable;
		typedef std::vector<SMSConfigurator*>  SMSConfiguratorList;
		
		Q_OBJECT
	public:
		ScoreEditorDlg(QWidget* parent=0);
		~ScoreEditorDlg();

		bool Apply();
		void SetTransformationScore(const CLAM::SMSTransformationChainConfig& sfg);
		const CLAM::SMSTransformationChainConfig& GetTransformationChain() const;

	private slots:
	    void onSelectAvailableTransformation(const QString&);
		void onSelectTransformationInScore(int);
		void addHighlightedToScore();
		void removeHighlightedFromScore();
		void moveHighlightedUp();
		void moveHighlightedDown();
		void scoreChanged();
		
	private:
		bool                               mIsShowed;
		bool                               mScoreChanged;
		CLAM::SMSTransformationChainConfig mTransformationChainCfg;
		HelpWidgetTable                    mHelpWidgetTable;
		SMSConfiguratorList                mSMSConfiguratorList;

		QBoxLayout* mTabPage0Layout;
		QBoxLayout* mTabPage1Layout;

		void GetAvailableTransformations();
		void SwapTransformations(int source, int dest);
		void HideHelpWidgets();
		void HideAll();
		void InitScoreEditor();
	};
}

#endif

