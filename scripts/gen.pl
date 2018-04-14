#!/usr/bin/perl
# Made by kk

use File::Find;

use strict;
use warnings;

die "Usage: $0 <vno-cli> <workdir> <times>" unless $#ARGV == 2;

my ($vno_cli, $wd, $times) = @ARGV;

$\ = "\n";
chdir $wd;
print STDERR "Do doings $times times in $wd...";

my @chars = ('a'..'z', 'A'..'Z');

sub rndstr {
  my $s = '';
  $s .= $chars[rand @chars] for 1..rand 15+2;
  $s;
}

sub touch_something {
  my $name = rndstr(35) . '.java';
  return '' if $name =~ /\.vno/;
  `touch $name`;
  " Create $name";
}

sub write_somewhere {
  my $content = rndstr 1000;
  my @files = ();
  find ( sub { push @files, $File::Find::name unless -d }, './' );
  my $file = $files[rand @files];
  return '' if $file =~ /\.vno/;
  `echo $content > $file`;
  " Edit $file"
}

sub delete_something {
  my @files = ();
  find ( sub { push @files, $File::Find::name unless -d }, './' );
  my $file = $files[rand @files];
  return '' if $file =~ /\.vno/;
  `rm $file`;
  " Delete $file"
}

sub do_something {
  my @deals = (
    \&touch_something,
    \&write_somewhere,
    \&delete_something,
  );
  $deals[rand @deals]->();
}

`$vno_cli init`;
my $i = 600;
my $out = `$vno_cli checkout $i 2>&1`;
while ($out =~ /ERROR 500/) {
	$i--;
	$out = `$vno_cli checkout $i 2>&1`;
	print STDERR ">>> $out <<<";;
}

for (1..$times) {
	my $msg = '';
	$msg .= do_something() for 1..rand(8)+2;
	`$vno_cli commit "$msg"`
}
