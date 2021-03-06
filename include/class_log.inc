<?php
/*
 * This code is part of GOsa (http://www.gosa-project.org)
 * Copyright (C) 2003-2008 GONICUS GmbH
 *
 * ID: $$Id$$
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*! \brief   The logging base class
  \author  Fabian Hickert <hickert@gonicus.de>
  \version 2.6
  \date    11.04.2007

  This is the base class for the GOsa logging functionality.
  All logging should lead to this class. 
 */
class log {

  var $config;

 /*! \brief logging constructor

    \param action         One of these values (modify|create|remove|snapshot|copy)
    \param objecttype     represents the current edited objecttype, like users/user
    \param object         represents the current edited object dn
    \param changes_array  An array containing names of all touched attributes
    \param result         A status message, containing errors or success messages 

    \sa log()
   */
  function __construct($action,$objecttype,$object,$changes_array = array(),$result = "")
  {
    if(!is_array($changes_array)){
      trigger_error("log(string,string,string,array(),bool). Forth parameter must be an array.");
      $changes_array = array();
    }

    $entry = array(); 
    if(!session::global_is_set('config')){
      $entry['user']= "unkown";
    }else{

      $this->config = session::global_get('config');
      $ui           = get_userinfo(); 
      $entry['user']= @$ui->dn;
    }

    /* Create string out of changes */
    $changes  ="";
    foreach($changes_array as $str ){
      $changes .= $str.",";
    }
    $changes = preg_replace("/,$/","",$changes );
    
    /* Create data object */
    $entry['timestamp'] = time();
    $entry['action']    = $action;
    $entry['objecttype']= $objecttype;
    $entry['object']    = $object;
    $entry['changes']   = $changes;
    $entry['result']    = $result;

    if(!isset($this->config) && empty($entry['user'])){
      $entry['user']  = "unknown";
    }

    /* Check if all given values are valid */
    global $config;
    $msgs = @log::check($entry);
    if(count($msgs)){
      foreach($msgs as $msg){
        trigger_error("Logging failed, reason was: ".$msg);
        msg_dialog::display(_("Internal error"), sprintf(_("Logging failed: %s"), $msg), ERROR_DIALOG);
      }
    }else{
      if(is_object($config) && $config->boolValueIsTrue("core","logging")){
        $this->log_into_syslog($entry);
      }
    }
  }


  function check($entry = array())
  {
    $msgs = array();

    if(!isset($entry['action']) || !in_array_strict($entry['action'],array("modify","create","remove","copy","snapshot","view","security","debug"))){
      $msgs[] = sprintf(_("Invalid option %s specified!"), bold($entry['action']));
    }

    if(!isset($entry['objecttype'])){
      $msgs[] = _("Specified 'objectType' is empty or invalid!");
    }
  
    return($msgs);
  }

   
  /* This function is used to into the systems syslog */
  function log_into_syslog($entry)
  {
    $str= "";

    if(!empty($entry['action']))        $str .= "({$entry['action']}) ";

    // Add object and object type if set.
    if(!empty($entry['object']) && !empty($entry['objecttype'])){
        $str .= "{$entry['object']} of type {$entry['objecttype']} ";
    }elseif(!empty($entry['object'])){
        $str .= "{$entry['object']} ";
    }elseif(!empty($entry['objecttype'])){
        $str .= "{$entry['objecttype']} ";
    }
    if(!empty($entry['changes'])) $str .= "{$entry['changes']} ";
    if(!empty($entry['result']))  $str .= ": {$entry['result']} ";

    gosa_log($str);
  }

}

// vim:tabstop=2:expandtab:shiftwidth=2:filetype=php:syntax:ruler:
?>
