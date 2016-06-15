#!/usr/bin/perl -w
################################################################
# A perl script to run the fault injection automaticlly
# Author : zhaozhilong 
# Email : zhaozhilong@ftcl.hit.edu.cn
################################################################

my $dir = "/home/ftcl/sjh/ptraceFI-x64";
#my $dir = ".";
my $cfgfile = $dir."/bin/test.conf";
my $wtype;
my $workload;
my $totalnum;
my $injector = $dir."/bin/injector";

#获得参数
sub getArg
{
	@arg = @_;
	my $i = 0;
	if(@arg != 6)
	{
		print "usage : more arguments is required\n";
		return -1;
	}
	
	if($arg[$i] eq "-c")
	{
		$cfgfile = $arg[++$i];
		++$i;
		#print $cfgfile."\n";
	}
	if($arg[$i] eq "-p" || $arg[$i] eq "-e")
	{
		$wtype = $arg[$i];
		$workload = $arg[++$i];
		++$i;
		#print $ptype."\n";
		#print $process."\n";
	}
	if($arg[$i] eq "-n")
	{
		$totalnum = $arg[++$i];
		#print $counter."\n";
	}
	if($i < 3)
	{
		print "unknown type of arguments\n";
		return -1;
	}
	return 0;
}

#执行故障注入
my $ret = getArg(@ARGV);
if($ret == -1)
{
	print "Get arguments error\n";
	return -1;
}

for($j = 0;$j < $totalnum;$j++)
{
	my $cmd = $injector." -c ".$cfgfile." ".$wtype." ".$workload;
	#print $cmd."\n";
	print "time:".($j+1)."\n";
	my $info = `$cmd`;
	print ($info."\n");
}
