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

class MakerDialog : public Gtk::Dialog
{
public:
    MakerDialog( BaseObjectType*, const Glib::RefPtr< Gtk::Builder >& );
    virtual ~MakerDialog();

protected:
    void onCreateClicked();
    void onCancelClicked();
    void onSelectDirClicked();

private:
    Glib::RefPtr< Gtk::Builder > mrBuilder;
    Gtk::Button* mpCreate;
    Gtk::Button* mpCancel;
    Gtk::Button* mpSelected;
    Gtk::Entry* mpTargetDir;

};

