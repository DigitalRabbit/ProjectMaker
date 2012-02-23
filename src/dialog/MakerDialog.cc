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

/*!
 * 各種 Widget instance を取得し、コールバック関数の登録を行っています。
 * 取得する Instance と 設定するコールバック関数は以下の通りです。
 * <table>
 *  <tr><th>Instance</th><th>Widget name</th><th>Menber</th><th>Callback function</th></tr>
 *  <tr align="center"><td>Gtk::Button</td><td>bt_create</td><td>yes</td><td>onCreateClicked</td></tr>
 *  <tr align="center"><td>Gtk::Button</td><td>bt_cancel</td><td>yes</td><td>onCancelClicked</td></tr>
 *  <tr align="center"><td>Gtk::FileChooserButton</td><td>fcb_project_dir</td><td>yes</td><td>onProjectDirSelected</td></tr>
 * </table>
 *
 * \param[in]   aCoject      基底クラス引渡し用 Object instance
 * \param[in]   aRefBuilder  GTK Builder instance
 */
MakerDialog::MakerDialog( BaseObjectType* aCobject, const Glib::RefPtr< Gtk::Builder >& aRefBuilder )
        : Gtk::Dialog( aCobject ), mrBuilder( aRefBuilder ), mpCreate( nullptr ), mpCancel( nullptr ), mpProjectDir( nullptr )
{
    // Get the Glade-instantiated Button, and connect a signal handler:
    // Create ボタン
    mrBuilder->get_widget( "bt_create", mpCreate );
    if( mpCreate != nullptr )
        mpCreate->signal_clicked().connect( sigc::mem_fun( *this, &MakerDialog::onCreateClicked ) );

    // Cancel ボタン
    mrBuilder->get_widget( "bt_cancel", mpCancel );
    if( mpCancel != nullptr )
        mpCancel->signal_clicked().connect( sigc::mem_fun( *this, &MakerDialog::onCancelClicked ) );

    // Project directory フォルダ選択ボタン
    mrBuilder->get_widget( "fcb_project_dir", mpProjectDir );
    if( mpProjectDir != nullptr )
    {
        mpProjectDir->signal_selection_changed().connect( sigc::mem_fun( *this, &MakerDialog::onProjectDirSelected ) );
        // 初期状態の選択フォルダでパースしてみる。
        onProjectDirSelected();
    }

    return;
}

/*! <b>現在未使用</b> */
MakerDialog::~MakerDialog() {}

/*!
 * <i>&lt;Create&gt;</i> ボタンクリック時のコールバック関数です。\n
 * 現在は何もしていませんが、将来的にはリリース物一式を作成するボタンとなる予定です。
 */
void MakerDialog::onCreateClicked()
{
    return;
}

/*!
 * <i>&lt;Cancel&gt;</i> ボタンクリック時のコールバック関数です。\n
 * アプリケーションを終了します。
 */
void MakerDialog::onCancelClicked()
{
    // hide() will cause main::run() to end.
    hide();
    return;
}

/*!
 * プロジェクト選択用フォルダ選択ボタンのコールバック関数です。\n
 * 選択されたフォルダーが変更されたタイミングで呼ばれ、\n
 * AndroidManifest.xml を読み込んで各種設定値をラベルへ貼り付けます。
 */
void MakerDialog::onProjectDirSelected()
{
    // AndroidManifest.xml の取得
    Glib::ustring filepath = mpProjectDir->get_filename();
    filepath += "/AndroidManifest.xml";

    Gtk::Label* pAppName, *pVersionName, *pVersionCode;
    mrBuilder->get_widget( "lb_app_name", pAppName );
    mrBuilder->get_widget( "lb_version_name", pVersionName );
    mrBuilder->get_widget( "lb_version_code", pVersionCode );

    // ファイル存在確認
    if( !Glib::file_test( filepath, Glib::FILE_TEST_EXISTS | Glib::FILE_TEST_IS_REGULAR ) )
    {
        pAppName->set_text( "This directory is not Android project." );
        pVersionName->set_text( "" );
        pVersionCode->set_text( "" );
        return;
    }

    try
    {
        // パーサー生成
        digirabi::android::ManifestParser parser( filepath );
//        parser.print_node( parser.getRootNode() );

        // 各ラベルへ取得値を設定
        pAppName->set_text( parser.getApplicationName() );
        pVersionName->set_text( parser.getVersionName() );
        pVersionCode->set_text( parser.getVersionCode() );
    }
    catch(const std::exception& ex)
    {
        std::cout << "Exception caught: " << ex.what() << std::endl;
    }

    return;
}

