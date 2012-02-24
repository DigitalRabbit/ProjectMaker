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

#include "MakerDialog.h"
#include "AndroidManifestParser.h"
#include <iostream>
#include <glibmm/arrayhandle.h>
#include <glibmm/iochannel.h>

/*!
 * 各種 Widget instance を取得し、コールバック関数の登録を行っています。\n
 * また、この時点で Console 表示用 TextView の背景色、前景色を変更します。\n
 * 取得する Instance と 設定するコールバック関数は以下の通りです。
 * <table>
 *  <tr><th>Instance</th><th>Widget name</th><th>Menber</th><th>Callback function</th></tr>
 *  <tr align="center"><td>Gtk::Button</td><td>bt_create</td><td>yes</td><td>onCreateClicked</td></tr>
 *  <tr align="center"><td>Gtk::Button</td><td>bt_cancel</td><td>yes</td><td>onCancelClicked</td></tr>
 *  <tr align="center"><td>Gtk::FileChooserButton</td><td>fcb_project_dir</td><td>yes</td><td>onProjectDirSelected</td></tr>
 * </table>
 *
 * \param[in]   aCobject     基底クラス引渡し用 Object instance
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
        // 初期状態の選択フォルダでパースする。
        onProjectDirSelected();
    }

    /* コンソール表示用 TextView を以下の通りに変更
     * 背景色 : 黒( #000000 )
     * 文字色 : 白( #FFFFFF )
     */
    Gtk::TextView* pTextView;
    mrBuilder->get_widget( "tv_console", pTextView );

    Gdk::RGBA color;
    color.set( "#000000" ); pTextView->override_background_color( color );
    color.set( "#FFFFFF" ); pTextView->override_color( color );

    return;
}

/*! <b>現在未使用</b> */
MakerDialog::~MakerDialog() {}

/*!
 * <i>&lt;Create&gt;</i> ボタンクリック時のコールバック関数です。\n
 * 現在は res/release.sh に選択したフォルダパスを指定して起動します。
 */
void MakerDialog::onCreateClicked()
{
    // 引数作成
    std::vector<Glib::ustring> argv;
    argv.push_back( "res/release.sh" );
    argv.push_back( mpProjectDir->get_filename() );

    /*! \note <span style="font-size: large;">子プロセスの非同期起動について</span>\n\n
     * 子プロセスの非同期起動には、 <b><i>Glib::spawn_async_with_pipes</i></b> を使用していますが、その引数は以下の通りです。\n
     * ( 戻り値は無し )\n
     * <table>
     *  <tr><th>Value type</th><th>Value name</th><th>In / Out</th><th>Description </th></tr>
     *  <tr><td>const std::string &amp;</td><td>working_directory</td><td>in</td><td>子プロセスの作業ディレクトリ </td></tr>
     *  <tr><td>const Glib::ArrayHandle&lt; std::string &gt;&amp;</td><td>argv</td><td>in</td><td>引数 </td></tr>
     *  <tr><td>Glib::SpawnFlags</td><td>flags = Glib::SpawnFlags(0)</td><td>in</td><td>起動フラグ </td></tr>
     *  <tr><td>const sigc::slot&lt; void &gt; &amp; </td><td>child_setup = sigc::slot&lt; void &gt;()</td><td>in</td><td>セットアップコールバック( 起動コールバック ) </td></tr>
     *  <tr><td>Glib::Pid *</td><td>child_pid = 0</td><td>out</td><td>子プロセスID </td></tr>
     *  <tr><td>int *</td><td>standard_input = 0</td><td>out</td><td>標準入力ファイルディスクリプタ </td></tr>
     *  <tr><td>int *</td><td>standard_output = 0</td><td>out</td><td>標準出力ファイルディスクリプタ </td></tr>
     *  <tr><td>int *</td><td>standard_error = 0</td><td>out</td><td>標準エラー出力ファイルディスクリプタ </td></tr>
     * </table>
     * <span style="color: red;">※.上記引数のうち、 <b><i>&lt;argv&gt;</i></b> に関しては、std::vector&lt;Glib::ustring&gt; で渡すと ArrayHandle&lt;T, Tr&gt; がオペレータを使って生成してくれるようです。</span>
     */
    int input, output, error;
    Glib::spawn_async_with_pipes( Glib::get_current_dir(), argv, Glib::SPAWN_SEARCH_PATH, sigc::slot<void>(), &mPid, &input, &output, &error );

    // 子プロセスの 標準 fd で IOChannell を作成
    mrOutput = Glib::IOChannel::create_from_fd( output );
    mrError = Glib::IOChannel::create_from_fd( error );

    // IOSource を用意して、コールバック関数を登録( 出力 / エラー )
    Glib::RefPtr<Glib::IOSource> source;

    /*! \todo どうしてこうしないとシグナルが飛んでこないのか調査中... */
    source = mrOutput->create_watch( Glib::IO_IN | Glib::IO_OUT );
    source->connect( sigc::mem_fun( *this, &MakerDialog::onStandardOutout ) );
    source->attach();

    source = mrError->create_watch( Glib::IO_IN | Glib::IO_OUT );
    source->connect( sigc::mem_fun( *this, &MakerDialog::onStandardError ) );
    source->attach();

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

/*!
 * 非同期子プロセスの標準出力をトレースするためのコールバック関数です。
 * メンバ変数で保管されている標準出力 IOChannel を読み込み用関数へ引き渡しています。
 */
bool MakerDialog::onStandardOutout( Glib::IOCondition aCondition )
{
    return ( readChannelLine( mrOutput ) );
}

/*!
 * 非同期子プロセスの標準エラーをトレースするためのコールバック関数です。
 * メンバ変数で保管されている標準出力 IOChannel を読み込み用関数へ引き渡しています。
 */
bool MakerDialog::onStandardError( Glib::IOCondition aCondition )
{
    return ( readChannelLine( mrError ) );
}

/*!
 * 与えられた IOChannel から 1 行分の文字列をコンソール用 TextBuffer へ出力します。
 *
 * \param[in]   arChannel   読み出し用 IOChannel instance
 *
 * \return      bool        読み出し結果
 * \retval      true        成功 
 * \retval      false       失敗
 */
bool MakerDialog::readChannelLine( Glib::RefPtr<Glib::IOChannel> arChannel )
{
    Gtk::TextView* pTextView;
    Glib::ustring line;

    // TextBuffer 取得
    mrBuilder->get_widget( "tv_console", pTextView );
    Glib::RefPtr<Gtk::TextBuffer> rBuffer = pTextView->get_buffer();

    // IOChannel から読み込んで TextBuffer へ引渡し
    arChannel->read_line( line );
    rBuffer->insert_at_cursor( line );

    return ( true );
}

