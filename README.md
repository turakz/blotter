<header>
    <h1>
        <b>blotter</b>
    </h1>
    <div>
        <ul>
            <li>a collection of (re)usable abstractions and tools for general C++ software development.</li>
        </ul>
    </div>
</header>
<body>
	<h2>
        <b>dependencies</b>
    </h2>
    <ul>
        <li>
       		<a href="https://www.boost.org">boost</a> (::asio, ::beast, etc.) 
        </li>
		<li>
			<a href="https://www.openssl.org/">OpenSSL</a>
		</li>
    </ul>
    <h2>
    	<b>docs</b>  
  	</h2>
  	<ul>
     	<li>
          	<!-- EVENTS BEGIN -->
            <b>::<a href="https://github.com/turakz/blotter/tree/master/events">events</b></a>: simple observer/subject interface for implementing an extendible events system using a minimal push-model 
          	<ul>
                <li><a href="https://github.com/turakz/blotter/tree/master/events/events.h">events.h</a></li>
                <li><a href="https://github.com/turakz/blotter/blob/master/events/tcp-client-with-events.cpp">async tcp client example</a></li>
          	</ul>
            <!-- EVENTS END -->
            <!-- STRATEGY BEGIN -->
            <b>::<a href="https://github.com/turakz/blotter/tree/master/events">strategy</b></a>: simple strategy interface for de-coupling behaviour from class hierarchies 
            <ul>
                <li><a href="https://github.com/turakz/blotter/blob/master/strategies/strategy.h">strategy.h</a></li>
                <li><a href="https://github.com/turakz/blotter/blob/master/strategies/strategy_example.cpp">strategies example: int vector sorting</a></li>
            </ul>
            <!-- STRATEGY END -->
      	</li>  
  	</ul>
</body>
