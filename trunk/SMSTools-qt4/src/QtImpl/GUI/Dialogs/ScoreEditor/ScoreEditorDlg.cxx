#include <algorithm>
#include <qlayout.h>
#include <qlistbox.h>
#include <qtabwidget.h>
#include "SMSConfigurator.hxx"
#include "ScoreEditorDlg.hxx"

namespace QtSMS
{
	ScoreEditorDlg::ScoreEditorDlg(QWidget* parent)
		: ScoreEditorDlgBase(parent)
		, mIsShowed(false)
		, mScoreChanged(false)
	{
		InitScoreEditor();
	}

	ScoreEditorDlg::~ScoreEditorDlg()
	{
		mHelpWidgetTable.clear();
		mSMSConfiguratorList.clear();
	}

    bool ScoreEditorDlg::Apply()
	{
		if(!mScoreChanged || !mSMSConfiguratorList.size()) return false;

		// first we insert the TransformationChainIO's at the beginning and at the end
		mTransformationChainCfg.GetConfigurations().clear();
		mTransformationChainCfg.GetOnArray().Resize(0);
		mTransformationChainCfg.GetOnArray().SetSize(0);

		CLAM::SMSTransformationChaineeConfig cfg;
		cfg.SetConcreteClassName("SMSTransformationChainIO");
		cfg.AddConcreteConfig();

		mTransformationChainCfg.GetConfigurations().push_back(cfg);
		mTransformationChainCfg.GetConfigurations().push_back(cfg);
		mTransformationChainCfg.GetOnArray().Resize(2);
		mTransformationChainCfg.GetOnArray().SetSize(2);
		mTransformationChainCfg.GetOnArray()[0] = mTransformationChainCfg.GetOnArray()[1] = true;

		for(unsigned i=0; i < mSMSConfiguratorList.size(); i++)
		{
			CLAM::SMSTransformationChaineeConfig cfg;
			cfg.SetConcreteClassName(mScoreTransformationList->text(i).ascii());
			cfg.SetConcreteConfig(mSMSConfiguratorList[i]->GetConfig());

			CLAM::SMSTransformationChainConfig::iterator it = mTransformationChainCfg.ConfigList_begin();
			// we insert the new configuration just before the 'ending' SMSTransformationChainIO
			std::advance(it, mTransformationChainCfg.ConfigList_size()-1);
			mTransformationChainCfg.GetConfigurations().insert(it, cfg);
			mTransformationChainCfg.GetOnArray().AddElem(1);
		}
		return true;
	}

	void ScoreEditorDlg::SetTransformationScore(const CLAM::SMSTransformationChainConfig& cfg)
	{
		mScoreChanged=false;
		CLAM::SMSTransformationChainConfig::const_iterator it = cfg.ConfigList_begin_const();
		it++;
		for(unsigned i=1; i < cfg.ConfigList_size()-1; i++, it++)
		{
			mScoreTransformationList->insertItem(it->GetConcreteClassName().c_str());
			QtSMS::SMSConfigurator* pCfg = QtSMS::SMSConfiguratorFactory::GetInstance().Create(it->GetConcreteClassName().c_str());
			pCfg->SetConfig(it->GetConcreteConfig());
			mSMSConfiguratorList.push_back(pCfg);
			mSMSConfiguratorList[i-1]->GetParametersWidget()->reparent(mTabWidget->page(1),QPoint(0,0));
			mTabPage1Layout->addWidget(mSMSConfiguratorList[i-1]->GetParametersWidget());
			mSMSConfiguratorList[i-1]->GetParametersWidget()->hide();
			connect(mSMSConfiguratorList[i-1],SIGNAL(configurationChanged()),SLOT(scoreChanged()));
		}
		HideAll();
		mAvailableTransformationList->clearSelection();
		mScoreTransformationList->clearSelection();
	}

	const CLAM::SMSTransformationChainConfig& ScoreEditorDlg::GetTransformationChain() const
	{
		return mTransformationChainCfg;
	}

	void ScoreEditorDlg::onSelectAvailableTransformation(const QString& str)
	{
		if(!mIsShowed)
		{
			mAvailableTransformationList->clearSelection();
			mIsShowed=true;
			return;
		}
		HideAll();
		mScoreTransformationList->clearSelection();
		mTabWidget->showPage(mTabWidget->page(0));
		mHelpWidgetTable[str.ascii()]->show();
	}

	void ScoreEditorDlg::onSelectTransformationInScore(int index)
	{
		HideAll();
		mAvailableTransformationList->clearSelection();
		mTabWidget->showPage(mTabWidget->page(1));
		mSMSConfiguratorList[index]->GetParametersWidget()->show();
		mHelpWidgetTable[mScoreTransformationList->text(index).ascii()]->show();
	}

	void ScoreEditorDlg::addHighlightedToScore()
	{
		QString str = mAvailableTransformationList->currentText();
		mScoreTransformationList->insertItem(str);
		mSMSConfiguratorList.push_back(QtSMS::SMSConfiguratorFactory::GetInstance().Create(str.ascii()));
		mSMSConfiguratorList[mSMSConfiguratorList.size()-1]->GetParametersWidget()->reparent(mTabWidget->page(1),QPoint(0,0));
		mTabPage1Layout->addWidget(mSMSConfiguratorList[mSMSConfiguratorList.size()-1]->GetParametersWidget());
		mScoreTransformationList->setSelected(mSMSConfiguratorList.size()-1,true);

		scoreChanged();
	}

	void ScoreEditorDlg::removeHighlightedFromScore()
	{
		if(!mScoreTransformationList->count()) return;
		int index = mScoreTransformationList->currentItem();
		if(mScoreTransformationList->count()==1) 
		{
			mScoreTransformationList->clear();
		}
		else
		{
			mScoreTransformationList->removeItem(index);
		}
		HideAll();
		SMSConfiguratorList::iterator pos = std::find(mSMSConfiguratorList.begin(),
													  mSMSConfiguratorList.end(),
													  mSMSConfiguratorList[index]);
		mTabPage1Layout->remove(mSMSConfiguratorList[index]->GetParametersWidget());
		delete mSMSConfiguratorList[index];
		mSMSConfiguratorList.erase(pos);
		if(mScoreTransformationList->count())
		{
			mScoreTransformationList->setSelected(mScoreTransformationList->currentItem(),true);
		}
		scoreChanged();
	}

	void ScoreEditorDlg::moveHighlightedUp()
	{
		int source = mScoreTransformationList->currentItem();
		if(!source) return;
		SwapTransformations(source,source-1);
	}

	void ScoreEditorDlg::moveHighlightedDown()
	{
		int source = mScoreTransformationList->currentItem();
		if(source == (int)mScoreTransformationList->count()-1) return;
		SwapTransformations(source,source+1);
	}

	void ScoreEditorDlg::scoreChanged()
	{
		if(!mScoreChanged) mScoreChanged=true;
	}

	void ScoreEditorDlg::GetAvailableTransformations()
	{
		mHelpWidgetTable.clear();
		mAvailableTransformationList->clear();
		
		std::list<std::string> availableTransformationsList;
		QtSMS::SMSConfiguratorFactory::GetInstance().GetRegisteredNames(availableTransformationsList);
		std::list<std::string>::const_iterator it = availableTransformationsList.begin();
		for(; it != availableTransformationsList.end(); it++)
		{
			mAvailableTransformationList->insertItem(it->c_str());
			mHelpWidgetTable[(*it)]=QtSMS::SMSConfiguratorFactory::GetInstance().Create(it->c_str())->GetHelpWidget();
			mHelpWidgetTable[(*it)]->reparent(mTabWidget->page(0),QPoint(0,0));
			mTabPage0Layout->addWidget(mHelpWidgetTable[(*it)]);
			mHelpWidgetTable[(*it)]->hide();
		}
	}

	void ScoreEditorDlg::SwapTransformations(int source, int dest)
	{
		QString str = mScoreTransformationList->text(source);
		mScoreTransformationList->changeItem(mScoreTransformationList->text(dest),source);
		mScoreTransformationList->changeItem(str,dest);
		SMSConfigurator* cfg = mSMSConfiguratorList[source];
		mSMSConfiguratorList[source]=mSMSConfiguratorList[dest];
		mSMSConfiguratorList[dest]=cfg;
		onSelectTransformationInScore(dest);
		scoreChanged();
	}

	void ScoreEditorDlg::HideHelpWidgets()
	{
		std::map<std::string,QWidget*>::const_iterator it = mHelpWidgetTable.begin();
		for(; it != mHelpWidgetTable.end(); it++)
		{
			it->second->hide();
		}
	}

	void ScoreEditorDlg::HideAll()
	{
		HideHelpWidgets();
		for(unsigned i=0; i < mSMSConfiguratorList.size(); i++)
		{
			mSMSConfiguratorList[i]->GetParametersWidget()->hide();
		}
	}

	void ScoreEditorDlg::InitScoreEditor()
	{
		mTabPage0Layout = new QVBoxLayout(mTabWidget->page(0));
		mTabPage1Layout = new QVBoxLayout(mTabWidget->page(1));
		GetAvailableTransformations();
	}
}

// END

