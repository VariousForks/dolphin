#include <qlabel.h>
#include <qlayout.h>
#include <qspinbox.h>
#include <qvgroupbox.h>
#include <qcheckbox.h>
#include <qwhatsthis.h>
#include <qgrid.h>

#include <kio/ioslave_defaults.h>
#include <ksaveioconfig.h>
#include <dcopclient.h>
#include <klocale.h>
#include <kdialog.h>
#include <kconfig.h>

#include "netpref.h"

#define MAX_TIMEOUT_VALUE  3600

KIOPreferences::KIOPreferences( QWidget* parent,  const char* name )
               :KCModule( parent, name )
{
    QVBoxLayout* mainLayout = new QVBoxLayout( this, KDialog::marginHint(),
                                               KDialog::spacingHint() );
    gb_Timeout = new QVGroupBox( i18n("Timeout Values"), this, "gb_Timeout" );
    QWhatsThis::add( gb_Timeout, i18n( "Here you can set timeout values. "
				       "You might want to tweak them if "
				       "your connection is very slow." ) );


    QGrid *grid = new QGrid(4, Qt::Vertical, gb_Timeout);
    grid->setSpacing(KDialog::spacingHint());

    QLabel* lbl_socket = new QLabel( i18n( "Soc&ket Read:" ), grid,
                                     "lbl_socket" );
    QLabel* lbl_proxy = new QLabel( i18n( "Pro&xy Connect:" ), grid,
                            "lbl_proxy" );

    QLabel* lbl_serverConnect = new QLabel( i18n("Server Co&nnect:"), grid,
                                            "lbl_serverConnect" );
    QLabel* lbl_serverResponse = new QLabel( i18n("Server &Response:"), grid,
                                             "lbl_serverResponse" );

    sb_socketRead = new QSpinBox( grid, "sb_socketRead" );
    sb_socketRead->setSuffix( i18n( " sec" ) );
    connect(sb_socketRead, SIGNAL(valueChanged ( int )),this, SLOT(configChanged()));

    sb_proxyConnect = new QSpinBox( grid, "sb_proxyConnect" );
    sb_proxyConnect->setSuffix( i18n( " sec" ) );
    connect(sb_proxyConnect, SIGNAL(valueChanged ( int )),this, SLOT(configChanged()));

    sb_serverConnect = new QSpinBox( grid, "sb_serverConnect" );
    sb_serverConnect->setSuffix( i18n( " sec" ) );
    connect(sb_serverConnect, SIGNAL(valueChanged ( int )),this, SLOT(configChanged()));

    sb_serverResponse = new QSpinBox( grid, "sb_serverResponse" );
    sb_serverResponse->setSuffix( i18n( " sec" ) );
    connect(sb_serverResponse, SIGNAL(valueChanged ( int )),this, SLOT(configChanged()));

	 QWidget *spacer = new QWidget(grid);
    spacer->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred));


    mainLayout->addWidget( gb_Timeout );



    gb_Ftp = new QVGroupBox( i18n( "FTP Options" ), this, "gb_Ftp" );
    cb_ftpEnablePasv = new QCheckBox( i18n( "Enable Passive &Mode (PASV)" ), gb_Ftp );
    mainLayout->addWidget( gb_Ftp );
    connect(cb_ftpEnablePasv, SIGNAL(toggled ( bool  )),this,SLOT(configChanged()));

    mainLayout->addStretch();

    lbl_socket->setBuddy( sb_socketRead );
    lbl_proxy->setBuddy( sb_proxyConnect );
    lbl_serverConnect->setBuddy( sb_serverConnect );
    lbl_serverResponse->setBuddy( sb_serverResponse );

    load();
}

KIOPreferences::~KIOPreferences()
{
}

void KIOPreferences::timeoutChanged(int /*val*/)
{
  emit changed(true);
}

void KIOPreferences::load()
{
  sb_socketRead->setValue( KProtocolManager::readTimeout() );
  sb_serverResponse->setValue( KProtocolManager::responseTimeout() );
  sb_serverConnect->setValue( KProtocolManager::connectTimeout() );
  sb_proxyConnect->setValue( KProtocolManager::proxyConnectTimeout() );

  sb_socketRead->setMinValue( MIN_TIMEOUT_VALUE );
  sb_serverResponse->setMinValue( MIN_TIMEOUT_VALUE );
  sb_serverConnect->setMinValue( MIN_TIMEOUT_VALUE );
  sb_proxyConnect->setMinValue( MIN_TIMEOUT_VALUE );

  sb_socketRead->setMaxValue( MAX_TIMEOUT_VALUE );
  sb_serverResponse->setMaxValue( MAX_TIMEOUT_VALUE );
  sb_serverConnect->setMaxValue( MAX_TIMEOUT_VALUE );
  sb_proxyConnect->setMaxValue( MAX_TIMEOUT_VALUE );

  KConfig config( "kio_ftprc", true, false );
  cb_ftpEnablePasv->setChecked( !config.readBoolEntry( "DisablePassiveMode", false ) );
}

void KIOPreferences::save()
{
  KSaveIOConfig::setReadTimeout( sb_socketRead->value() );
  KSaveIOConfig::setResponseTimeout( sb_serverResponse->value() );
  KSaveIOConfig::setConnectTimeout( sb_serverConnect->value() );
  KSaveIOConfig::setProxyConnectTimeout( sb_proxyConnect->value() );

  KConfig config( "kio_ftprc", false, false );
  config.writeEntry( "DisablePassiveMode", !cb_ftpEnablePasv->isChecked() );
  config.sync();

  emit changed(true);

  // Inform running io-slaves about change...
  QByteArray data;
  QDataStream stream( data, IO_WriteOnly );
  stream << QString::null;
  if ( !kapp->dcopClient()->isAttached() )
    kapp->dcopClient()->attach();
  kapp->dcopClient()->send( "*", "KIO::Scheduler",
                            "reparseSlaveConfiguration(QString)", data );
}

void KIOPreferences::defaults()
{
  sb_socketRead->setValue( DEFAULT_READ_TIMEOUT );
  sb_serverResponse->setValue( DEFAULT_RESPONSE_TIMEOUT );
  sb_serverConnect->setValue( DEFAULT_CONNECT_TIMEOUT );
  sb_proxyConnect->setValue( DEFAULT_PROXY_CONNECT_TIMEOUT );

  cb_ftpEnablePasv->setChecked( true );

  emit changed(true);
}

QString KIOPreferences::quickHelp() const
{
  return i18n("<h1>Network Preferences</h1>Here you can define"
              " the behavior of KDE programs when using Internet"
              " and network connections. If you experience timeouts"
              " and problems or sit behind a modem, you might want"
              " to adjust these values." );
}

#include "netpref.moc"
