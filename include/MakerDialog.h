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
 * \file   MakerDialog.h
 *
 * \author  Digital Rabbit(digital.rabbit.jp@gmail.com)
 * \date    2012/02/18
 */
#pragma once

#include    <gtkmm.h>
#include    <libxml++/libxml++.h>

/*! \brief  アプリケーションメインダイアログクラス
 *
 * ProjectMaker のメイン画面となるダイアログクラスです。\n
 * Taget directory や Create button 等の UI 制御を行います。\n
 *
 * \author  Digital Rabbit
 * \since   2012/02/18
 */
class MakerDialog : public Gtk::Dialog
{
public:
    /*! \brief コンストラクタ */
    MakerDialog( BaseObjectType* aCobject, const Glib::RefPtr< Gtk::Builder >& aRefBuilder );
    /*! \brief デストラクタ */
    virtual ~MakerDialog();

protected:
    /*! \brief <i>&lt;Create&gt;</i> ボタンクリック */
    void onCreateClicked();
    /*! \brief <i>&lt;Cancel&gt;</i> ボタンクリック */
    void onCancelClicked();
    /*! \brief Target directory フォルダ選択 */
    void onProjectDirSelected();
    /*! \brief 標準出力コールバック */
    bool onStandardOutout( Glib::IOCondition aCondition );
    /*! \brief 標準エラーコールバック */
    bool onStandardError( Glib::IOCondition aCondition );

private:
    /*! \brief GTK+ Builder */
    Glib::RefPtr< Gtk::Builder > mrBuilder;
    /*! \brief <i>&lt;Create&gt;</i> ボタン */
    Gtk::Button* mpCreate;
    /*! \brief <i>&lt;Cancel&gt;</i> ボタン */
    Gtk::Button* mpCancel;
    /*! \brief Project directory フォルダ選択ボタン */
    Gtk::FileChooserButton* mpProjectDir;

    /*! \brief シェル起動時子プロセスID */
    Glib::Pid mPid;

    /*! \brief 子プロセス標準出力 IO チャンネル */
    Glib::RefPtr<Glib::IOChannel> mrOutput;
    /*! \brief 子プロセス標準エラー IO チャンネル */
    Glib::RefPtr<Glib::IOChannel> mrError;

    /*! \brief チャンネル1行読み込み */
    bool readChannelLine( Glib::RefPtr<Glib::IOChannel> arChannel );

};

