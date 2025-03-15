filename = "example.txt";
fid = fopen (filename, "w");
fprintf (fid, "This is a line.\n");
fprintf (fid, "This is another line.\n");
fclose (fid);
