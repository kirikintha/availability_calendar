<?php
/**
 * @name availabilityCalendar
 *  Provides the functions for the calendar, so we can abstract these in the future.
 * @file class.availabilityCalendar.inc
 * @version 1.1
 */
class availabilityCalendar {
  /**
   * Class Variables.
   * @var string $path
   *  The path variable for the module.
   */
  public $path;
  /**
   * @internal
   *  Construct the availability Calendar object when we load the file.
   *  Destroy the Class object when we do not need it.
   */
  function __construct() {
    $this->path = AVAILABILITY_CALENDAR_MODULE_PATH;
  }
  function __destruct() { 
    unset($this);
  }

  /**
   * @static _loadJS().
   *  Loads the calendar listener javascript.
   */
  public static function _loadJS() {
    drupal_add_js(AVAILABILITY_CALENDAR_MODULE_PATH. '/js/functions.availabilityCalendar.js','module','header');
  }
  
  /**
   * @static _buildCalendar().
   *  Builds a 12 Month Calendar, this is an independent function, and is merely a Container for your display.
   *
   * @return html
   *  An html representation of a twelve month calendar.
   */
  public static function _buildCalendar() {
    $calendar = new availabilityCalendarCalendar;
    return $calendar->_render();
  }
  
  /**
   * @name _getDaysInBetween($start, $end, $status).
   *  Get days in between the startDate and endDate.
   * @param string $start
   * @param string $end
   * @param string $status
   * return array $days
   */
  public function _getDaysInBetween($start, $end, $status) {
    //Variables.
    $day     = 86400; // Day in seconds
    $format  = 'n-j-Y'; // Output format we need this to be js compatible
    $sTime 	 = strtotime($start); // Start as time
    $eTime 	 = strtotime($end); // End as time
    $numDays = round(($eTime - $sTime)  / $day) + 1;
    $days 	 = array();
    //Get days.
    for ($d = 0; $d < $numDays; $d++) {
      $date = date($format, ($sTime + ($d * $day)));
      $days[$date] = $status;
    }
    //When we have the array of times, we need to make sure that we add the "mid-day" class to the elements at the beginning and ending, so we can style them if necessary
    reset($days);
    $firstDay = key($days);
    $days[$firstDay] = $days[$firstDay] .' mid-day-first';
    
    end($days);
    $lastDay = key($days);
    $days[$lastDay] = $days[$lastDay] .' mid-day-last';
     
    // Return $days[dd-mm-yyyy] = $status
    return $days;
  }
  
  /**
   * @name _buildCalendarJS($days = NULL).
   *  Build our javascript calendar object.
   *  Output the js into a Drupal.settings.availabilityCalendar object.
   */
  public static function _buildCalendarJS($days = NULL) {
    //If we do not have days, return FALSE.
    if (empty($days)) { return FALSE; }
    //Build our javascript output.
    $js = "";
    $js .= "Drupal.settings.availabilityCalendar = {\n";
    //Foreach $days, make our availability variables.
    foreach ($days as $key => $value) {
      $data[] = "'$key' : '$value'";
    }
    //Implode the data.
    $js .= implode(",\n", $data);
    //Finish our object.
    $js .= "\n}";
    //Add to the javascript.
    drupal_add_js($js, 'inline', 'header');
  }
  
}

class availabilityCalendarCalendar {
  /**
   * Class Variables.
   * @var string $path
   *  We need the path for any module callbacks.
   * @var string $year
   *  We Need the Current year.
   * @var $months
   *  An Array of all months for your Calendar, This ifor Gregorian Calendars only!
   */
  private $path;
  private $year;
  private $months;
  /**
   * @internal
   * Construct/Destruct our class object.
   * Right now this is only for Gregorian Calendars.
   * @todo - change this to a plug-in of some sort, so we can handle different types of calendars.
   */
  function __construct() {
    //Local path, easier to find this way.
    $this->path = AVAILABILITY_CALENDAR_MODULE_PATH;
    //This year.
    $this->year = date('Y');
    //Months array.
    $months = array();
    $i = 0;
    while ($i < 12) {
      //Set month, monthName and Year based on our current month and year.
      $month     = date('m', strtotime("now +$i months"));
      $monthName = date('F', strtotime("now +$i months"));
      $year      = date('Y', strtotime("now +$i months"));
      //Increment and add a label to this month list.
      $i++;
      $months[$month] = strtotime("$i $monthName $year");
    }
    //Set months for the availability calendar to use.
    $this->months = $months;
    //Set the //Gregorian Days.
    $this->days = array(
      0 => t('Sun'),
      1 => t('Mon'),
      2 =>t('Tue'),
      3 => t('Wed'),
      4 => t('Thu'),
      5 => t('Fri'),
      6 => t('Sat'),
    );
  }
  function __destruct() {
    unset($this);
  }
  /**
   * @name _render
   *  Renders a twelve month Calendar, by itself this does nothing but give you a cacheable/cached calendar.
   */
  function _render() {
    //Get an array for all months of the year - I think we can even cache this right here @todo Cache this array?
    $months = self::_getMonths();
    //Get the days.
    $headers = $this->days;
    //Translate rows into months.
    foreach ($months as $month) {
      $rows  = array();
      foreach ($month['weeks'] as $key => $week) {
        //Make a valid calendar row.
        $rows[] = array('data' => $week, 'class' => 'week-' .$key);
      }
      //This is definitely the point I would like to allow for a better intercept of the table. @todo _preprocess of some sort.
      if (!empty($rows)) {
        //Render our final table.
        $calendar = theme('table', $headers, $rows, array('id' => $month['month_id'], 'class' => 'availability-calendar'));
        $data     .= theme('availability_calendar_calendar_month', $month['month_id'], $month['month_full_name'], $calendar);
      }
    }
    return $data;
  }
  /**
   * @name _getMonths()
   *  Get the months from the current month to a month oneyear int he future.
   * @return array
   *  An array of months that follow the current month to one year in the future.
   */
  function _getMonths() {
    $months = array();
    //Get all months into their proper array positions here!
    foreach ($this->months as $month => $time) {
      //Year.
      $year = date('Y', $time);
      //Total days.
      $months[$time]['total_days']      = date("t", mktime(0, 0, 0, $month, 1, $year));
      //Calculate what day of the week does this month start.
      $months[$time]['day_offest']      = date("w", mktime(0, 0, 0, $month, 1, $year));
      //Make the common short three letter name the id.
      $months[$time]['month_id']        = strtolower(date("M", mktime(0, 0, 0, $month, 1, $year)));
      //Full month name, for labels and such.
      $months[$time]['month_full_name'] = ucwords(date("F Y", mktime(0, 0, 0, $month, 1, $year)));
      
      $data = array();
      //Add in the empty table elements, so we don't orhpan tables.
      for ($i = 0; $i < $months[$time]['day_offest']; $i++) {
        $data[] = null;
      }
      //Get this date as a string, it's easier to play with daes this way using string to time.
      $str = date("d F Y", $time);
      for ($j = 0; $j < $months[$time]['total_days']; $j++) {
        //Get the date we need from string to time.
        $date = strtotime("$str +$j days");
        $day  = date("d", $date);
        $id   = date("n-j-Y", $date);
        //Add that calendar date to the days for the calendar.
        $data[] = array('data' => theme('availability_calendar_calendar_day', trim($day)), 'id' => $id, 'class' => 'calendar-day');
      }
      //Chunk data into a seven day calendar.
      $data     = array_chunk($data, 7, true);
      $end      = count($data) - 1;
      $catch_up = 7 - count(end($data));
      //Now, if we need to add to the end of the array, finish making all the elements the same count, because we are making tables.
      if ($catch_up > 0) {
        for ($i = 0; $i < $catch_up; $i++) {
          array_push($data[$end], null);
        }
      }
      //Days into a weeks row, so we can make rows from one "weeks" entry.
      $months[$time]['weeks'] = $data;
      
    }
    return $months;
  }
  
}