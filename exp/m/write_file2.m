filename = "example1.txt";
fid = fopen (filename, "w");
fprintf (fid, "This is a line.\n");
fprintf (fid, "This is another line.\n");
pct = 37;
fprintf (fid,"Processed %d%% of '%s'.\nPlease be patient.\n",pct, filename);
fclose (fid);