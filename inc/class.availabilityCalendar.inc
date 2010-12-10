<?php

class availabilityCalendar {
  
  public $path;
  
  function __construct() { //Build a new object so we can get what we need
    
    $this->path = AVAILABILITY_CALENDAR_MODULE_PATH;
    
  }
  
  function __destruct() { //Unset this, when it needs to be killed off
    
    unset($this);
    
  }
  
  /**
   * @name _buildCalendar
   * Builds a 12 Month Calendar, this is an independent function, and is merely a Container for your display.
   * @return html, an html representation of a twelve month calendar
   */
  public static function _buildCalendar() {
    
    $calendar = new availabilityCalendarCalendar;
    return $calendar->_render();
    
  }
  
  /**
   * @name _getDaysInBetween($start, $end, $status)
   * Get days in between the startDate and endDate
   * @param string $start
   * @param string $end
   * @param string $status
   * return array $days
   */
  public function _getDaysInBetween($start, $end, $status) {
    
    // Vars  
    $day     = 86400; // Day in seconds
    $format  = 'n-j-Y'; // Output format we need this to be js compatible
    $sTime 	 = strtotime($start); // Start as time
    $eTime 	 = strtotime($end); // End as time
    $numDays = round(($eTime - $sTime)  / $day) + 1;
    $days 	 = array();
    
    // Get days  
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
   * Output the js into a Drupal.propertyAvailability script
   */
  public static function _buildCalendarJS($days = NULL) {
    
    if (empty($days)) { return; }
    
    $js = "";
    $js .= "Drupal.availabilityCalendar = {\n";

    foreach ($days as $key => $value) {
      
      $data[] = "'$key' : '$value'";
      
    }
    
    $js .= implode(",\n", $data);
    
    $js .= "\n}";
    
    drupal_add_js($js, 'inline', 'header');
    
  }
  
}

class availabilityCalendarCalendar {
  
  private $path; //We need the path for any module callbacks
  private $year; //We Need the Current year.
  private $months; //An Array of all months for your Calendar, This ifor Gregorian Calendars only!
  
  function __construct() { //Build a new object so we can get what we need
    
    $this->path = AVAILABILITY_CALENDAR_MODULE_PATH; //Local path, easier to find this way
    
    $this->year = date('Y'); //This year

    $months = array();
    $i = 0;
    while ($i < 12) {
      //Set month, monthName and Year based on our current month and year.
      $month     = date('m', strtotime("now +$i months"));
      $monthName = date('F', strtotime("now +$i months"));
      $year      = date('Y', strtotime("now +$i months"));
      $months[$month] = strtotime("01 $monthName $year");
      $i++;
    }
    //Set months for the availability calendar to use.
    $this->months = $months;
    
    $this->days = array(
      0 => t('Sun'),
      1 => t('Mon'),
      2 =>t('Tue'),
      3 => t('Wed'),
      4 => t('Thu'),
      5 => t('Fri'),
      6 => t('Sat')
   ); //Gregorian Days
    
  }
  
  function __destruct() { //Unset this, when it needs to be killed off
    
    unset($this);
    
  }
  
  /**
   * @name _render
   * Renders a twelve month Calendar, by itself this does nothing but give you a cacheable/cached calendar
   */
  function _render() {
    
    //Get an array for all months of the year - I think we can even cache this right here @todo Cache this here array?
    $months = self::_getMonths();
    
    $headers = $this->days;
    
    //translate rows into months
    foreach ($months as $month) {
      
      $rows  = array();
      
      foreach ($month['weeks'] as $key => $week) {
        
        $rows[] = array('data' => $week, 'class' => 'week-' .$key);
        
      }
      
      if (!empty($rows)) { //This is definitely the point I would like to allow for a better intercept of the table
        
        $calendar = theme_table($headers, $rows, array('id' => $month['month_id'], 'class' => 'availability-calendar')); // our final table
        $data     .= theme('availability_calendar_calendar_month', $month['month_id'], $month['month_full_name'], $calendar);
        
      }
      
    }
    
    return $data;
        
  }
  
  function _getMonths() {
    
    $months = array();
    
    foreach ($this->months as $month => $time) { //get all months into their proper array positions here!
      
      $months[$time]['total_days']      = date("t", mktime(0, 0, 0, $month, 1, $this->year));//total days
      $months[$time]['day_offest']      = date("w", mktime(0, 0, 0, $month, 1, $this->year));//What day of the week does this month start?
      $months[$time]['month_id']        = strtolower(date("M", mktime(0, 0, 0, $month, 1, $this->year))); //make the common short three letter name the id
      $months[$time]['month_full_name'] = ucwords(date("F", mktime(0, 0, 0, $month, 1, $this->year))); //full name, for labels and such
      
      $data = array();
      
      for ($i = 0; $i < $months[$time]['day_offest']; $i++) { //Add in the empty table elements, so we don't orhpan tables
        
        $data[] = null;
        
      }
      
      $str = date("d F Y", $time); //Get this as a string, it's easier to play with daes this way
      for ($j = 0; $j < $months[$time]['total_days']; $j++) {
        
        $date = strtotime("$str +$j days");
        $day  = date("d", $date);
        $id   = date("n-j-Y", $date);
        
        $data[] = array('data' => theme('availability_calendar_calendar_day', trim($day)), 'id' => $id, 'class' => 'calendar-day');
        
      }
      
      $data     = array_chunk($data, 7, true); //Chunk data
      $end      = count($data) - 1;
      $catch_up = 7 - count(end($data));
      if ($catch_up > 0) { //Now, if we need to add to the end of the array, finish making all the elements the same count, because we are making tables
        
        for ($i = 0; $i < $catch_up; $i++) {
          
          array_push($data[$end], null);
          
        }
        
      }

      $months[$time]['weeks'] = $data;
      
    }
    
    return $months;
    
  }
  
}