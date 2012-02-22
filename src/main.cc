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
 * \file   main.cc
 *
 * \author  Digital Rabbit(digital.rabbit.jp@gmail.com)
 * \date    2012/02/18
 */

#include    "MakerDialog.h"
#include    <iostream>

/*! \brief エントリポイント
 *
 * ProjectMaker のエントリポイントです。\n
 * Gtk::Builder で glade ファイルからダイアログを生成、\n
 * GTK+ を使用してメインループへダイアログを引き渡しています。
 *
 * \param[in]   argc    コマンドライン引数の数
 * \param[in]   argv    コマンドライン引数配列
 *
 * \return      int     プログラム終了状態
 * \retval      EXIT_SUCCESS    正常終了
 * \retval      EXIT_FAILURE    異常終了
 */
int main( int argc, char* argv[] )
{
    // Create Main roop instance.
    Gtk::Main kit( argc, argv );

    // Load the Glade file and instiate its widgets:
    Glib::RefPtr< Gtk::Builder > builder = Gtk::Builder::create();
    try
    {
        builder->add_from_file( "res/main_dialog.glade" );
    }
    catch( const Glib::FileError& ex )
    {
        std::cerr << "FileError: " << ex.what() << std::endl;
        return 1;
    }
    catch( const Glib::MarkupError& ex )
    {
        std::cerr << "MarkupError: " << ex.what() << std::endl;
        return 1;
    }
    catch( const Gtk::BuilderError& ex )
    {
        std::cerr << "BuilderError: " << ex.what() << std::endl;
        return 1;
    }

    // Create dialog.
    MakerDialog* pDialog = nullptr;
    builder->get_widget_derived( "dl_main", pDialog );
    if( pDialog != nullptr )
    {
        // Run dialog.
        kit.run( *pDialog );
        delete pDialog;
    }

    return ( EXIT_SUCCESS );
}

