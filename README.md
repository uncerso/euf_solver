# Solver of theories of uninterpreted functions with equality

# Requirements
<code>g++</code> at least 7th version and <code>make</code>

# How to build
<code>make build</code>

# How to use
<code>./run <<< "$formula"</code>

# Examples
<code>./run <<< 'x = y & f(x,y) = x & f(y,x) != y'</code>

<code>./run <<< 'g(g(x,y,z),z) = x & g(x,y,z) = x & g(x,z) != x'</code>
(function overloading is available, hence <code>g</code> with <code>2</code> and <code>3</code> arguments are different functions)

More examples you can see in the <code>sat_tests</code> and <code>unsat_tests</code> files

# Operations
logical 'and' is <code>&</code>

equality is <code>=</code>

inequality is <code>!=</code>
