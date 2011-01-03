<?php
/**
 * @file theme.sno_country_client.inc
 * Sno Country Client Theme
 * Handles all the theme functions
 * Version: 1.0
 */

//Sno Country Preprocess
function template_preprocess_sno_country_client_block(&$vars) {
  //Return $widget variables to the template, then you can use the HTML as you wish.
  $vars['widget']['id'] = $id;
  //Return the proper widget from the processor. 
  $vars = _snow_country_client_process_url($vars);
}