# this program will scan the specified text file and write a summary file containing the structure

open(DB,"< browsedb.txt");
@db = <DB>;
close(DB);
foreach $line (@db) {
	#print $line;
	$line =~ s/\n//g;
	my @parts = split("\t",$line);
	$name_dob = $parts[0];
	$studydatetime = $parts[1];
	$seriesnum = $parts[2];
	$filepath = $parts[3];
#	print $name_dob;

	$numfiles = $dbhash{$name_dob}{$studydatetime}{$seriesnum}{'numfiles'};
	$dbhash{$name_dob}{$studydatetime}{$seriesnum}{'files'}[$numfiles] = $filepath;
	$dbhash{$name_dob}{$studydatetime}{$seriesnum}{'numfiles'}++;
#	print @files;
}

open(PDB,"> browsepdb.txt");
foreach $name_dob (sort keys %dbhash) {
	%studyhash = $dbhash{$name_dob};
	print PDB "$name_dob\n";
	foreach $studydatetime (sort keys %{ $dbhash{$name_dob} }) {
		print PDB "\t$studydatetime\n";
		foreach $seriesnum (sort keys %{ $dbhash{$name_dob}{$studydatetime} }) {
			print PDB "\t\t$seriesnum\n";
			foreach $i (0..$dbhash{$name_dob}{$studydatetime}{$seriesnum}{'numfiles'}) {
				print PDB "\t\t\t" . $dbhash{$name_dob}{$studydatetime}{$seriesnum}{'files'}[$i] . "\n";
			}
		}
	}
}
close(PDB);