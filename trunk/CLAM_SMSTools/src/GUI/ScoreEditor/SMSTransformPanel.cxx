#include "SMSTransformPanel.hxx"
#include "SMSTransformationConfig.hxx"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Select_Browser.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Tabs.H>
#include <FL/Fl_Box.H>
#include <FL/fl_ask.H>
#include "Factory.hxx"
#include "SMS_Configurator.hxx"
#include <iostream>

#define FL_DEFINED_UP_BOX FL_UP_BOX
#define FL_DEFINED_DOWN_BOX FL_DOWN_BOX

inline void SMSScoreEditor::cb_mMoveTransUpInScoreButton_i(Fl_Button*, void*) {
	MoveHighlightedUp();
}
void SMSScoreEditor::cb_mMoveTransUpInScoreButton(Fl_Button* o, void* v) {
	((SMSScoreEditor*)(o->parent()->parent()->user_data()))->cb_mMoveTransUpInScoreButton_i(o,v);
}

inline void SMSScoreEditor::cb_mMoveTransDownInScoreButton_i(Fl_Button*, void*) {
	MoveHighlightedDown();
}
void SMSScoreEditor::cb_mMoveTransDownInScoreButton(Fl_Button* o, void* v) {
	((SMSScoreEditor*)(o->parent()->parent()->user_data()))->cb_mMoveTransDownInScoreButton_i(o,v);
}

inline void SMSScoreEditor::cb_mRemoveTransFromScoreButton_i(Fl_Button*, void*) {
	RemoveHighlightedFromScore();
}
void SMSScoreEditor::cb_mRemoveTransFromScoreButton(Fl_Button* o, void* v) {
	((SMSScoreEditor*)(o->parent()->parent()->user_data()))->cb_mRemoveTransFromScoreButton_i(o,v);
}

inline void SMSScoreEditor::cb_mAddTransformToScoreButton_i(Fl_Button*, void*) {
	AddHighlightedToScore();
}
void SMSScoreEditor::cb_mAddTransformToScoreButton(Fl_Button* o, void* v) {
	((SMSScoreEditor*)(o->parent()->parent()->user_data()))->cb_mAddTransformToScoreButton_i(o,v);
}

inline void SMSScoreEditor::cb_mDiscardChangesButton_i(Fl_Button*, void*) {
	DiscardChangesAndClose();
}
void SMSScoreEditor::cb_mDiscardChangesButton(Fl_Button* o, void* v) {
	((SMSScoreEditor*)(o->parent()->user_data()))->cb_mDiscardChangesButton_i(o,v);
}

inline void SMSScoreEditor::cb_mApplyChangesButton_i(Fl_Button*, void*) {
	ApplyChangesAndClose();
}

void SMSScoreEditor::cb_mApplyChangesToCurrentCfg(Fl_Button* o, void* v) {
	((SMSScoreEditor*)v)->cb_mApplyChangesToCurrentCfg_i(o,v);
}

inline void SMSScoreEditor::cb_mApplyChangesToCurrentCfg_i(Fl_Button*, void*) {

}

void SMSScoreEditor::cb_mApplyChangesButton(Fl_Button* o, void* v) {
	((SMSScoreEditor*)(o->parent()->user_data()))->cb_mApplyChangesButton_i(o,v);
}

inline void SMSScoreEditor::cb_mRepositoryBrowser_i(Fl_Select_Browser* b, void*) 
{
	if ( b->value() > b->size() || b->value() < 1 )
	     return;
	
	mScoreContentsBox->deselect( mHighlightedConfig );
	mHighlightedConfig = 0;
	
	ClearTransformationWidgets();
	SetTransformationHelpWidget( *((CLAMVM::SMSConfigurator*)b->data( b->value() )));	
}

void SMSScoreEditor::cb_mRepositoryBrowser(Fl_Select_Browser* o, void* v)
{
	((SMSScoreEditor*)v)->cb_mRepositoryBrowser_i(o,v);
}

inline void SMSScoreEditor::cb_mScoreBrowser_i(Fl_Select_Browser* b, void*)
{
	if ( b->value() > b->size() || b->value() < 1 )
	     return;
	if ( mHighlightedConfig == b->value() )
		return;
	
	mHighlightedConfig = b->value();
	b->select( mHighlightedConfig );
	
	ClearTransformationWidgets();
	CLAMVM::SMSConfigurator& selectedConfigurator = *((CLAMVM::SMSConfigurator*)( b->data(b->value() )));

	SetTransformationHelpWidget( selectedConfigurator );
	SetTransformationEditWidget( selectedConfigurator );
	
	mTransTabs->redraw();

}

void SMSScoreEditor::cb_mScoreBrowser(Fl_Select_Browser* o, void* v)
{
	((SMSScoreEditor*)v)->cb_mScoreBrowser_i(o,v);
}



SMSScoreEditor::SMSScoreEditor()
	: mHighlightedConfig( 0 )
{
	Fl_Window* w;

	{
		Fl_Window* o = mMainWindow = new Fl_Window(890, 475, "SMS - Transformation Score Edition");
		w = o;
		o->box(FL_FLAT_BOX);
		o->user_data((void*)(this));

		{
			Fl_Group* o = mChainTopologyDock = new Fl_Group(5, 20, 455, 420,"Score");
			o->labelfont(FL_HELVETICA);
			o->labelsize(14);
			o->align(FL_ALIGN_TOP);
			o->box(FL_ENGRAVED_BOX);
			{
				Fl_Select_Browser* o =
					mScoreContentsBox =
					new Fl_Select_Browser(235, 40, 220, 365, "Transformations in the Score");
				o->labelsize(12);
				o->textsize(12);
				o->callback( (Fl_Callback*)cb_mScoreBrowser, this );
				o->align(FL_ALIGN_TOP);
			}
			{
				Fl_Select_Browser* o =
					mRepositoryBox =
					new Fl_Select_Browser(10, 40, 220, 365, "Available SMS Transformations");
				o->labelsize(12);
				o->textsize(12);
				o->callback( (Fl_Callback*)cb_mRepositoryBrowser, this );
				o->align(FL_ALIGN_TOP);
			}
			{
				Fl_Button* o =
					mMoveTransUpInScoreButton =
					new Fl_Button(235, 410, 65, 25, "Move &up");
				o->tooltip("Move selected transformation up in the score");
				o->box(FL_DEFINED_UP_BOX);
				o->down_box(FL_DEFINED_DOWN_BOX);
				o->shortcut(0x80075);
				o->callback((Fl_Callback*)cb_mMoveTransUpInScoreButton);
				o->labelsize(12);
			}
			{
				Fl_Button* o =
					mMoveTransDownInScoreButton =
					new Fl_Button(305, 410, 85, 25, "Move &down");
				o->tooltip("Move selected transformation down in the score");
				o->box(FL_DEFINED_UP_BOX);
				o->down_box(FL_DEFINED_DOWN_BOX);
				o->shortcut(0x80064);
				o->callback((Fl_Callback*)cb_mMoveTransDownInScoreButton);
				o->labelsize(12);
			}
			{
				Fl_Button* o =
					mRemoveTransFromScoreButton =
					new Fl_Button(395, 410, 60, 25, "&Remove");
				o->tooltip("Remove selected transformation from score");
				o->box(FL_DEFINED_UP_BOX);
				o->down_box(FL_DEFINED_DOWN_BOX);
				o->shortcut(0x80072);
				o->callback((Fl_Callback*)cb_mRemoveTransFromScoreButton);
				o->labelsize(12);
			}
			{
				Fl_Button* o =
					mAddTransformToScoreButton =
					new Fl_Button(10, 410, 220, 25, "Add &transformation to score");
				o->tooltip("Adds selected transformation to score");
				o->box(FL_DEFINED_UP_BOX);
				o->down_box(FL_DEFINED_DOWN_BOX);
				o->shortcut(0x80074);
				o->callback((Fl_Callback*)cb_mAddTransformToScoreButton);
				o->labelsize(12);
			}
			o->end();
		} // Score group

		{
			Fl_Button* o =
				mApplyChangesToScoreButton =
				new Fl_Button(5, 445, 225, 25, "&Apply Changes to Score");
			o->box(FL_DEFINED_UP_BOX);
			o->down_box(FL_DEFINED_DOWN_BOX);
			o->shortcut(0x80061);
			o->callback( (Fl_Callback*)cb_mApplyChangesButton );
			o->deactivate();
			o->labelsize(12);
		}
		{
			Fl_Button* o =
				mDiscardChangesButton =
				new Fl_Button(235, 445, 225, 25, "&Discard Changes to Score");
			o->box(FL_DEFINED_UP_BOX);
			o->down_box(FL_DEFINED_DOWN_BOX);
			o->shortcut(0x80064);
			o->callback((Fl_Callback*)cb_mDiscardChangesButton);
			o->labelsize(12);
		}

		{
			Fl_Group* o =
				mTransParmDock =
				new Fl_Group(465, 20, 420, 420, "Transformation");
			o->labelfont(FL_HELVETICA);
			o->labelsize(14);
			o->align(FL_ALIGN_TOP);
			o->box(FL_ENGRAVED_BOX);

			{
				Fl_Tabs* o = mTransTabs = new Fl_Tabs(470, 25, 410, 410);
				o->box(FL_DEFINED_UP_BOX);
				o->labelsize(12);

				{
					Fl_Group* o =
						mHelpWidgetContainer =
						new Fl_Group( mTransTabs->x()+5, mTransTabs->y()+30, mTransTabs->w()-10, mTransTabs->h()-35 );
					o->label( "Description" );
					o->labelsize( 12 );
					o->box( FL_FLAT_BOX );
					o->end();
				}
				{
					Fl_Group* o =
						mConfigWidgetContainer =
						new Fl_Group( mTransTabs->x()+5, mTransTabs->y()+30, mTransTabs->w()-10, mTransTabs->h()-35 );
					o->label( "Parameters" );
					o->labelsize( 12 );
					o->box( FL_FLAT_BOX );
					o->end();
					o->deactivate();
				}

				o->end();
			} // tabs
			o->end();
		} // Transformation Group

		{
			Fl_Button* o =
				mApplyChangesToCurrentCfg =
				new Fl_Button(580, 445, 150, 25, "Apply configuration");
			o->tooltip("Applies configuration to the selected transformation");
			o->box(FL_DEFINED_UP_BOX);
			o->down_box(FL_DEFINED_DOWN_BOX);
			o->callback( (Fl_Callback*) cb_mApplyChangesToCurrentCfg, this );
			o->deactivate();
			o->labelsize(12);
		}
		{
			Fl_Button* o =
				mDiscardConfigButton =
				new Fl_Button(735, 445, 150, 25, "Discard configuration");
			o->box(FL_DEFINED_UP_BOX);
			o->down_box(FL_DEFINED_DOWN_BOX);
			o->deactivate();
			o->labelsize(12);
		}
		{
			mNoConfigWidgetAvailable = new Fl_Box( 0, 0, 100, 100 );
			mNoConfigWidgetAvailable->label( "Non-editable configuration" );
			mNoConfigWidgetAvailable->align( FL_ALIGN_INSIDE );
			mNoConfigWidgetAvailable->hide();
		}


		o->set_modal();
		o->end();
	}


	SetTransformationScore.Wrap( this, &SMSScoreEditor::OnSetTransformationScore );
	ScoreWasChanged.Wrap( this, &SMSScoreEditor::OnScoreChanged );
	ShowFactoryProductsOnBrowser();
	ResetChangedStatus();
}

void SMSScoreEditor::OnScoreChanged( )
{
	mUserChangedSomething = true;
	mApplyChangesToScoreButton->activate();
}

void SMSScoreEditor::OnSetTransformationScore( const CLAM::SMSTransformationChainConfig& cfg )
{
	ShowScoreOnBrowser(cfg);
}

SMSScoreEditor::~SMSScoreEditor()
{
	if ( mNoConfigWidgetAvailable->parent() == NULL )
		delete mNoConfigWidgetAvailable;
}

void SMSScoreEditor::ResetChangedStatus()
{
	mUserChangedSomething = false;
}

void SMSScoreEditor::Show( )
{
	mApplyChangesToScoreButton->deactivate();
	mMainWindow->show();
}

void SMSScoreEditor::Hide( )
{
	ResetChangedStatus();
	mHighlightedConfig = 0;
	mMainWindow->hide();
}

void SMSScoreEditor::ShowFactoryProductsOnBrowser()
{
	mRepositoryBox->clear();
	std::list< std::string > availableTransformationsList;

	CLAMVM::SMSConfiguratorFactory::GetInstance().GetRegisteredNames( availableTransformationsList );

	std::list< std::string >::const_iterator i = availableTransformationsList.begin();

	for ( ; i != availableTransformationsList.end(); i++ )
	{
		mRepositoryBox->add( i->c_str() );
		mRepositoryBox->data( mRepositoryBox->size(),
			CLAMVM::SMSConfiguratorFactory::GetInstance().Create( i->c_str() ) );
	}

}

void SMSScoreEditor::ShowScoreOnBrowser(  const CLAM::SMSTransformationChainConfig& cfg )
{
	mScoreContentsBox->clear();
	CLAM::SMSTransformationChainConfig::const_iterator i = cfg.ConfigList_begin_const();

	i++;
	for ( int k = 1; k < cfg.ConfigList_size()-1; k++ )
	{
		if ( cfg.GetOnArray()[k] )
		{
			mScoreContentsBox->add( i->GetConcreteClassName().c_str() );
		}
		else
		{
			std::string italicToken = "@i";
			italicToken += i->GetConcreteClassName().c_str();
			mScoreContentsBox->add( italicToken.c_str() );
		}
		CLAMVM::SMSConfigurator* pCfg = CLAMVM::SMSConfiguratorFactory::GetInstance().Create( i->GetConcreteClassName().c_str() );
		pCfg->SetConfig( i->GetConcreteConfig() );
		pCfg->ConfigurationChanged.Connect( ScoreWasChanged );
		mScoreContentsBox->data( mScoreContentsBox->size(), pCfg );
		i++;
	}

}

void SMSScoreEditor::DiscardChangesAndClose()
{
	if ( !IsScoreChanged() )
	{
		Hide();
		return;
	}
	int answer = fl_ask( "Are you sure to discard score changes?" );


	if ( answer )
	{
		Hide();
	}

}

void SMSScoreEditor::ApplyChangesAndClose()
{
	if ( !IsScoreChanged() )
	{
		fl_message( "The score has not been changed" );
		Hide();
		return;
	}

	int answer = fl_ask( "Are you sure to apply score changes?" );

	if ( answer )
	{
		ApplyChangesIntoChain();
		Hide();
	}
}

void SMSScoreEditor::InsertNewConfiguratorInScoreBox( const char* transName )
{
	mScoreContentsBox->add( transName );

	std::string key = transName;

	CLAMVM::SMSConfigurator* pCfg = CLAMVM::SMSConfiguratorFactory::GetInstance().Create( key );

	mScoreContentsBox->data( mScoreContentsBox->size(), pCfg );

	mHighlightedConfig = mScoreContentsBox->size();

	// we 'activate' the configurator by showing it on the GUI
	ClearTransformationWidgets();
	SetTransformationHelpWidget( *pCfg );
	SetTransformationEditWidget( *pCfg );

	// Connect 
	pCfg->ConfigurationChanged.Connect( ScoreWasChanged );

	// we highlight the configuration just added
	mScoreContentsBox->select( mHighlightedConfig );
}

void SMSScoreEditor::InsertConfigurationIntoChain( const char* name, const CLAM::ProcessingConfig& userConfig,
						   CLAM::SMSTransformationChainConfig& chain )
{
	std::string givenName = name;
	
	// search for special Fl_Browser chars
	if ( givenName[0] == '@' )
	{
		givenName = givenName.substr( 2, givenName.size()-2 );
	}
	else
		givenName = name;

	CLAM::ProcessingChaineeConfig cfg;
	cfg.SetConcreteClassName( givenName.c_str() );
	cfg.SetConcreteConfig( userConfig );

	CLAM::SMSTransformationChainConfig::iterator i = chain.ConfigList_begin();
	// we insert the new configuration just before the 'ending' SMSTransformationChainIO
	std::advance( i, chain.ConfigList_size()-1 );
	chain.GetConfigurations().insert( i, cfg );
	chain.GetOnArray().AddElem( 1 );
	
}

void SMSScoreEditor::AddHighlightedToScore( )
{
	if ( mRepositoryBox->size() == 0 ) // there are no configurators available
		return;

	// let's retrieve the transformation selected by the user
	int insertedTransformation = mRepositoryBox->value();
	const char* transformationName =  mRepositoryBox->text( insertedTransformation );

	InsertNewConfiguratorInScoreBox( transformationName );

	// turn 'on' the 'dirty' boolean
	ScoreWasChanged();
}

void SMSScoreEditor::ClearTransformationHelpWidget()
{
	CLAM_ASSERT( mHelpWidgetContainer->children() <= 1, "Help widget container had more than one widget" );
	
	if ( mHelpWidgetContainer->children() > 0 ) // no widgets to hide
	{
	
		Fl_Widget* currentHelpWidget = mHelpWidgetContainer->child(0);
		CLAM_ASSERT( currentHelpWidget != NULL, "Current help widget was NULL!" );
		currentHelpWidget->hide();
		mHelpWidgetContainer->remove( *currentHelpWidget );
		mMainWindow->add(currentHelpWidget );
		mHelpWidgetContainer->end();
	}

	mHelpWidgetContainer->deactivate();
	mTransTabs->redraw();
}

void SMSScoreEditor::ClearTransformationEditWidget()
{
	CLAM_ASSERT( mConfigWidgetContainer->children() <= 1, "Config widget container had more than widget" );
	if ( mConfigWidgetContainer->children() > 0 )
	{
		Fl_Widget* currentConfigWidget = mConfigWidgetContainer->child(0);
		CLAM_ASSERT( currentConfigWidget !=NULL, "Current config widget was NULL!" );
		currentConfigWidget->hide();
		mConfigWidgetContainer->remove( *currentConfigWidget );
		mMainWindow->add(currentConfigWidget);
		mConfigWidgetContainer->end();
		
	}
	mConfigWidgetContainer->deactivate();
	mTransTabs->redraw();
}

void SMSScoreEditor::SetTransformationHelpWidget( CLAMVM::SMSConfigurator& conf )
{
	CLAM_ASSERT( conf.GetHelpWidget() != NULL, "Configurator did not provide help widget" );
	CLAM_ASSERT( mHelpWidgetContainer->children() == 0, "Help widget container was not empty" );
	
	Fl_Widget* helpWidget = conf.GetHelpWidget();
	
	Fl_Group* currentParent = helpWidget->parent();
	if ( currentParent )
	{
		currentParent->remove( helpWidget );
		currentParent->end();
	}
	mHelpWidgetContainer->add( helpWidget );
	mHelpWidgetContainer->end();
	helpWidget->resize( mHelpWidgetContainer->x(), mHelpWidgetContainer->y()+5,
			    mHelpWidgetContainer->w(), mHelpWidgetContainer->h()-5 );
	helpWidget->show();
	mHelpWidgetContainer->activate();
	mTransTabs->value( mHelpWidgetContainer );
}

void SMSScoreEditor::SetTransformationEditWidget( CLAMVM::SMSConfigurator& conf )
{
	CLAM_ASSERT( mConfigWidgetContainer->children() == 0, "Config widget container was not empty" );
	Fl_Widget* configWidget = conf.GetParametersWidget();
	
	if ( configWidget == NULL )
	{
		configWidget = mNoConfigWidgetAvailable;
	}

	Fl_Group* currentParent = configWidget->parent();
	if ( currentParent )
	{
		currentParent->remove( configWidget );
		currentParent->end();
	}
	mConfigWidgetContainer->add( configWidget );
	mConfigWidgetContainer->end();
	configWidget->resize( mConfigWidgetContainer->x(), mConfigWidgetContainer->y()+5,
			      mConfigWidgetContainer->w(), mConfigWidgetContainer->h()-5 );

	mConfigWidgetContainer->activate();
	configWidget->show();
	mTransTabs->value( mConfigWidgetContainer );
}

void SMSScoreEditor::RemoveConfiguratorFromScoreBox( int index )
{
	CLAMVM::SMSConfigurator* cfg = (CLAMVM::SMSConfigurator*)mScoreContentsBox->data( index );

	CLAM_ASSERT( mHelpWidgetContainer->children() <= 1, "Too many children for the help widget container" );
	if ( mHelpWidgetContainer->children() )
		if ( cfg->GetHelpWidget() == mHelpWidgetContainer->child(0) )
			ClearTransformationHelpWidget();
	Fl_Group* currentParent = cfg->GetHelpWidget()->parent();
	if ( currentParent )
	{
		cfg->GetHelpWidget()->hide();
		currentParent->remove( cfg->GetHelpWidget() );
		currentParent->end();
	}

	CLAM_ASSERT( mConfigWidgetContainer->children() <=1, "Too many children for the config widget container" );
	if ( mConfigWidgetContainer->children() )
		if ( cfg->GetParametersWidget() == mConfigWidgetContainer->child(0) )
			ClearTransformationEditWidget();
	
	currentParent =cfg->GetParametersWidget()->parent();
	if ( currentParent)
	{
		cfg->GetParametersWidget()->hide();
		currentParent->remove( cfg->GetParametersWidget() );
		currentParent->end();
	}

	CLAM_ASSERT( cfg!=NULL, "The browser row did not have a configurator!" );
	delete cfg;

	mScoreContentsBox->data( index, NULL );
	mScoreContentsBox->remove(  index );

}

void SMSScoreEditor::RemoveHighlightedFromScore( )
{
	if ( mHighlightedConfig == 0 ) // no selected configuration
		return;

	RemoveConfiguratorFromScoreBox( mHighlightedConfig );

	ScoreWasChanged();

	mHighlightedConfig = 0;

}

void SMSScoreEditor::SwapConfigurators( int source, int destination )
{
	mScoreContentsBox->move( source,destination );
	mScoreContentsBox->select( destination );
}

void SMSScoreEditor::MoveHighlightedDown()
{
	int destination = mScoreContentsBox->value()+1;
	int source = mScoreContentsBox->value();

	if ( destination > mScoreContentsBox->size() )
		return;

	SwapConfigurators( source, destination );

	ScoreWasChanged();
	mHighlightedConfig++;
}

void SMSScoreEditor::MoveHighlightedUp() 
{
	int destination = mScoreContentsBox->value()-1;
	int source = mScoreContentsBox->value();

	if ( destination < 1 )
		return;

	SwapConfigurators( source, destination );

	mHighlightedConfig--;
	ScoreWasChanged();
}

void SMSScoreEditor::ApplyChangesIntoChain()
{
	CLAM::SMSTransformationChainConfig newChainConfig;

	// first we insert the TransformationChainIO's at the beginning and at the end
	newChainConfig.GetConfigurations().clear();
	newChainConfig.GetOnArray().Resize(0);
	newChainConfig.GetOnArray().SetSize(0);

	CLAM::ProcessingChaineeConfig cfg;
	cfg.SetConcreteClassName( "SMSTransformationChainIO" );
	cfg.AddConcreteConfig();

	newChainConfig.GetConfigurations().push_back(  cfg  );
	newChainConfig.GetConfigurations().push_back(  cfg );
	newChainConfig.GetOnArray().Resize( 2 );
	newChainConfig.GetOnArray().SetSize( 2 );
	newChainConfig.GetOnArray()[0] = newChainConfig.GetOnArray()[1] = true;


	for ( int i = 1; i <= mScoreContentsBox->size(); i++ )
	{
		CLAMVM::SMSConfigurator* pCfg = ( CLAMVM::SMSConfigurator* )mScoreContentsBox->data(i);
		CLAM_ASSERT( pCfg != NULL, "Invalid configurator!" );
		InsertConfigurationIntoChain( mScoreContentsBox->text(i), pCfg->GetConfig(), newChainConfig );
	}

	TransformationChainChanged.Emit( newChainConfig );

}

