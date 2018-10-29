# MiniTorrentSystem
A peer-to-peer file sharing system, based on the distributed architecture. 
Client requests a file, and portions of the requested file residing or peer machines are sent to the client, and then reassembled into a copy of the requested file.
A special tracker server keeps track of which file is shared by which client.


Minimalistic .mtorrent file.
Terminologies
Seeder
Leecher
Peer

Design Decison
Chunk size
Not too big- because we have to send that over network, loss of large packet will be more costly.
Not too small- no. of request will be too much.

Client is multi-threaded,so that multiple requests can be handlded.

Format of .mtorrent file:
● Tracker URL1: The url of the tracker server1, i.e. tracker1 IP & port
● Tracker URL2: The url of the tracker server2, i.e. tracker2 IP & port
● Filename : the name with which the file would be saved on disk.
● Filesize : Complete file size
● Hash String: String whose length would be a multiple of 20. This string is a
concatenation of SHA1 hashes of each piece of file.


The "tracker" server  will keep track of all the available peer machines onwhich the files or its parts reside.
After the initial peer-to-peer file download is started, peer-to-peer communication can continue
without the connection to a tracker.


Functionalities of torrent client:
1.Share files to the tracker and generate a corresponding “.mtorrent” file
2.Retrieve peer information from tracker upon providing a “.mtorrent” file.
3.Download files from multiple peers simultaneously, same goes for upload.
4.Maintain mapping of paths of files and related .mtorrent files.

