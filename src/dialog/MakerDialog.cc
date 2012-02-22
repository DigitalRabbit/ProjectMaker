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
 *  <tr>               <th>Instance</th>    <th>Widget name</th>   <th>Menber</th> <th>Callback function</th>  </tr>
 *  <tr align="center"><td>Gtk::Button</td> <td>bt_create</td>     <td>yes</td>    <td>onCreateClicked</td>    </tr>
 *  <tr align="center"><td>Gtk::Button</td> <td>bt_cancel</td>     <td>yes</td>    <td>onCancelClicked</td>    </tr>
 *  <tr align="center"><td>Gtk::Button</td> <td>bt_select_dir</td> <td>yes</td>    <td>onSelectDirClicked</td> </tr>
 *  <tr align="center"><td>Gtk::Entry</td>  <td>et_target_dir</td> <td>yes</td>    <td align="center">-</td>   </tr>
 * </table>
 *
 * \param[in]   aCoject      基底クラス引渡し用 Object instance
 * \param[in]   aRefBuilder  GTK Builder instance
 */
MakerDialog::MakerDialog( BaseObjectType* aCobject, const Glib::RefPtr< Gtk::Builder >& aRefBuilder )
        : Gtk::Dialog( aCobject ), mrBuilder( aRefBuilder ), mpCreate( nullptr ), mpCancel( nullptr ), mpSelected( nullptr ), mpTargetDir( nullptr )
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

    // Select... ボタン
    mrBuilder->get_widget( "bt_select_dir", mpSelected );
    if( mpSelected != nullptr )
        mpSelected->signal_clicked().connect( sigc::mem_fun( *this, &MakerDialog::onSelectDirClicked ) );

    // Target direcotry エントリ
    mrBuilder->get_widget( "et_target_dir", mpTargetDir );

    return;
}

/*! <b>現在未使用</b> */
MakerDialog::~MakerDialog() {}

/*!
 * <i>&lt;Create&gt;</i> ボタンクリック時のコールバック関数です。\n
 * 現在は <i>&lt;Android&gt;</i> タブ内のラベルへ package, versionName, versionCode を表示していますが、\n
 * 将来的にはリリース物一式を作成するボタンとなる予定です。
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
 * <i>&lt;Select...&gt;</i> ボタンクリック時のコールバック関数です。\n
 * フォルダ選択ダイアログをモーダルで起動し、\n
 * 選択されたフォルダパスを Target directory エントリーへ設定します。
 */
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

