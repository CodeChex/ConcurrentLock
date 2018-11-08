
In order to use resource locks effectively,
the developer must take into account 2 variables:

1) Success Rate:

	An application should have a priority defined for actually obtaining
	a resource, and also have the proper operation when it cannot access
	the desired resource.

2) Access Time:

	An application needs to also take into account the actual time it takes 
	to guarantee a certain level of access.

This benchmark can be used to gauge the effectiveness of different
types of locks on a resource.  

Some notes on using the benchmark tool:

Notice when using the "critsect" lock, all requests are successful, 
but the average access time for the resource is huge.  This can result
in a very slow application.

When using a "mutex", not all requests are successful, and those that
are successful are highly variable in the access time.  These factors
are sensitive to the estimates cycle time of each type of access as well
as the timeout allotted to access the resource.

When using "concurrent locks", the factors in determining a good success
rate and access time are even more sensitive to the above variables.  These
locks are the only ones that differentiate between "read" (shared) and 
"write" (dedicated) access modes.

Feel free to contact me with any questions about this benchmark application.


John C. Checco
checco@checco.com