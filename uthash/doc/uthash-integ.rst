UTHash Integration with ns-3: A Simple Guide
=====================================

.. include:: replace.txt
.. highlight:: cpp

What is UTHash?
--------------
UTHash is a simple hash table library that helps you store and find data quickly in your ns-3 simulations. Think of it like a dictionary where you can look up values using keys.

What Can You Do With It?
-----------------------
1. Store and find data quickly
2. Create routing tables for network simulations
3. Keep track of connections between network nodes

Basic Usage Example
-----------------
Here's a simple example of how to use UTHash in your ns-3 code:

.. code-block:: cpp

    // Create a routing table
    HashTableHelper helper;
    auto routeTable = helper.CreateRoutingTable(node);
    
    // Add a route (like telling a router where to send packets)
    helper.AddRoute(routeTable, "10.1.1.2", "10.1.1.1", 0, 10);
    
    // Find where to send a packet
    std::string nextHop = helper.FindNextHop(routeTable, destAddress);
    
    // Show all routes
    helper.PrintRoutingTable(routeTable);

Limitations
----------
- Only works with string and integer keys
- Basic features only (no fancy sorting)

Testing Your Code
---------------
To test if everything works:

.. code-block:: bash

    ./ns3 test uthash-integ

Where to Find Examples
--------------------
Look in these files for examples:
- ``examples/uthash-integ-example.cc``
- ``uthash-point-to-point.cc``

Need More Help?
-------------
Check out: https://troydhanson.github.io/uthash/
