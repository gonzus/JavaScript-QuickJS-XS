use strict;
use warnings;

use Test::More;

my $CLASS = 'JavaScript::QuickJS::XS';

sub main {
    use_ok($CLASS);
    done_testing;
    return 0;
}

exit main();
