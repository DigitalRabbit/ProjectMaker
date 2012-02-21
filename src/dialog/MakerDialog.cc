/*-************************************************************
 *  ____            _        _   ____       _     _       _
 * |  _ \(_) __ _(_) |_ __ _| | |  _ \ __ _| |__ | |__ (_) |_
 * | | | | |/ _` | | __/ _` | | | |_) / _` | '_ \| '_ \| | __|
 * | |_| | | (_| | | |_ (_| | | |  _ < (_| | |_) | |_) | | |_
 * |____/|_|\__, |_|\__\__,_|_| |_| \_\__,_|_.__/|_.__/|_|\__|
 *          |___/
 **************************************************************
 * Copyright(C) 2012 Digital Rabbit All Rights Reserved.  *//*!
 *
 * \file   MakerDialog.cc
 *
 * \author  Digital Rabbit(digital.rabbit.jp@gmail.com)
 * \date    2012/02/18
 */

#include    "MakerDialog.h"
#include    "AndroidManifestParser.h"
#include    <iostream>

MakerDialog::MakerDialog( BaseObjectType* cobject, const Glib::RefPtr< Gtk::Builder >& refBuilder )
        : Gtk::Dialog( cobject ), mrBuilder( refBuilder ), mpCreate( nullptr ), mpCancel( nullptr ), mpSelected( nullptr ), mpTargetDir( nullptr )
{
    //Get the Glade-instantiated Button, and connect a signal handler:
    mrBuilder->get_widget( "bt_create", mpCreate );
    if( mpCreate != nullptr )
        mpCreate->signal_clicked().connect( sigc::mem_fun( *this, &MakerDialog::onCreateClicked ) );

    mrBuilder->get_widget( "bt_cancel", mpCancel );
    if( mpCancel != nullptr )
        mpCancel->signal_clicked().connect( sigc::mem_fun( *this, &MakerDialog::onCancelClicked ) );

    mrBuilder->get_widget( "bt_select_dir", mpSelected );
    if( mpSelected != nullptr )
        mpSelected->signal_clicked().connect( sigc::mem_fun( *this, &MakerDialog::onSelectDirClicked ) );

    mrBuilder->get_widget( "et_target_dir", mpTargetDir );

    return;
}

MakerDialog::~MakerDialog()
{
}

/*!
 * Create Button Click.
 */
void MakerDialog::onCreateClicked()
{
    // AndroidManifest.xml の取得
    Glib::ustring filepath = mpTargetDir->get_text();
    filepath += "/AndroidManifest.xml";

    // ファイル存在確認
    if( !Glib::file_test( filepath, Glib::FILE_TEST_EXISTS | Glib::FILE_TEST_IS_REGULAR ) )
    {
        return;
    }

    try
    {
        // パーサー生成
        digirabi::android::ManifestParser parser( filepath );
//        parser.print_node( parser.getRootNode() );

        Gtk::Label* pWorker = nullptr;
        mrBuilder->get_widget( "lb_app_name", pWorker );
        pWorker->set_text( parser.getApplicationName() );

        mrBuilder->get_widget( "lb_version_name", pWorker );
        pWorker->set_text( parser.getVersionName() );

        mrBuilder->get_widget( "lb_version_code", pWorker );
        pWorker->set_text( parser.getVersionCode() );
    }
    catch(const std::exception& ex)
    {
        std::cout << "Exception caught: " << ex.what() << std::endl;
    }

    return;
}

void MakerDialog::onCancelClicked()
{
    // hide() will cause main::run() to end.
    hide();
    return;
}

void MakerDialog::onSelectDirClicked()
{
    Gtk::FileChooserDialog dialog( "フォルダ選択", Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER );
    dialog.set_transient_for( *this );

    // Add response buttons the the dialog:
    dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
    dialog.add_button("選択", Gtk::RESPONSE_OK);

    // Dialog open and handle the response:
    switch( dialog.run() )
    {
    case Gtk::RESPONSE_OK:
        mpTargetDir->set_text( dialog.get_filename() );
        break;

    default:
        break;
    }

    return;
}

