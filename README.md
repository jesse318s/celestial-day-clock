# celestial-day-clock

This project defines a clock class that keeps track of time in a celestial day. It includes a simplified version of std::numeric_limits with additional functionality for an unspecified type and a time type (std::time_t). The clock class demonstrates the use of the new numeric limits of time functionality.

## About Representing Time

&nbsp; &nbsp; &nbsp; &nbsp; Time (not date-time) can be reduced to a minimum of 2 radices (6 and 10) as a 
hierarchical unit system such as when written for a timer, clock, or stopwatch. The way this is 
done is by recording the radices when time is written digit by digit. <br/>
&nbsp; &nbsp; &nbsp; &nbsp; When writing time without days or other date units, each digit is written having the 
highest possible value of 5 or 9. This indicates that the radices for this context are 6 and 10. (2 
total radices) For comparison, another possible radix pair for time could be 60 and 10, but the 
problem with this pair is that the radices come from two conceptually different forms of time, 
which are the time units minutes and seconds and the single digits of other time units such as 
milliseconds or the count of hours. <br/>
&nbsp; &nbsp; &nbsp; &nbsp; So, although time is sometimes classified as base 60, it’s actually base 6 and base 10. The 
implications for this concept include creating new clocks for other celestial bodies that don’t 
disrupt the mathematical advantages of our base 6 and base 10 time unit system. The way this is 
done is by first, starting at zero, appending X/2 minutes to the AM and PM clock sections for 
every X minutes of the body’s full rotation. However, in order for the clock to be symmetrical, 
the number of minutes that belong to the AM and PM sections must be even.