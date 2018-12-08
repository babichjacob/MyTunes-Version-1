#include "Song.h"
#include "ID3v2.h"


Song::Song() {
}


Song::Song(filesystem::directory_entry path) {
	set_from_file(path);
}


Song::~Song()
{
}


string get_frame_data_as_string(const uint8_t *data, int data_length) {
	string data_str = "";
	int first_alphanumerical_index = -1;

	// Hunt down the first index of an alphanumeric character (because everything before it will be garbage)
	for (int i = 0; i < data_length; i++) {
		if (isalnum(data[i])) {
			first_alphanumerical_index = i;
			break;
		}
	}

	// Then add all the characters after it to the string
	// (it occurred to me to use a substr method but I don't think that'll work because this isn't really a string or char array)
	for (int i = first_alphanumerical_index; i < data_length; i++) {
		if ((data[i] != 0))	data_str += data[i];
	}

	return data_str;
}


void Song::set_from_file(filesystem::directory_entry path) {
	cout << "Song::set_from_file: about to set from file " << path.path().string() << endl;

	ID3v2::Tag mp3_tag(path.path().string());

	if (!mp3_tag.IsValid()) {
		cout << "Song::set_from_file: this is not a valid MP3 file -- bye" << endl;
		throw runtime_error("invalid song");
	}
	
	// Since the constructor will exit from the throw statement above if the file is invalid (not an MP3),
	// we can continue on the assumption that we're working with a valid file
	is_valid = true;

	// Extract all the useful data (these can't all be turned into their own methods
	// because that was giving me a null pointer exception)
	ID3v2::Frame::v23::TIT2 *tit2 = dynamic_cast<ID3v2::Frame::v23::TIT2 *>(mp3_tag.GetFrameWithName("TIT2"));
	if (tit2 != nullptr) title = get_frame_data_as_string(tit2->GetData(), tit2->GetSize());
	else title = "Unknown Song";

	ID3v2::Frame::v23::TALB *talb = dynamic_cast<ID3v2::Frame::v23::TALB *>(mp3_tag.GetFrameWithName("TALB"));
	if (talb != nullptr) album = get_frame_data_as_string(talb->GetData(), talb->GetSize());
	else album = "Unknown Album";

	ID3v2::Frame::v23::TPE1 *tpe1 = dynamic_cast<ID3v2::Frame::v23::TPE1 *>(mp3_tag.GetFrameWithName("TPE1"));
	if (tpe1 != nullptr) artist = get_frame_data_as_string(tpe1->GetData(), tpe1->GetSize());
	else artist = "Unknown Artist";

	ID3v2::Frame::v23::TCON *tcon = dynamic_cast<ID3v2::Frame::v23::TCON *>(mp3_tag.GetFrameWithName("TCON"));
	if (tcon != nullptr) genre = get_frame_data_as_string(tcon->GetData(), tcon->GetSize());
	else genre = "Unknown Genre";

	ID3v2::Frame::v23::TRCK *trck = dynamic_cast<ID3v2::Frame::v23::TRCK *>(mp3_tag.GetFrameWithName("TRCK"));
	if (trck != nullptr) track_of_album = get_frame_data_as_string(trck->GetData(), trck->GetSize());
	else track_of_album = "0/0";

	ID3v2::Frame::v23::TYER *tyer = dynamic_cast<ID3v2::Frame::v23::TYER *>(mp3_tag.GetFrameWithName("TYER"));
	if (tpe1 != nullptr) year = stoi(get_frame_data_as_string(tyer->GetData(), tyer->GetSize()));
	else year = 1900;
}


void Song::print() {
	for (int i = 0; i < 40; i++) cout << "-";
	cout << endl;

	cout << title << " (" << track_of_album << "): " << endl;
	cout << "\t Artist: " << artist << endl;
	cout << "\t Album: "  << album << endl;
	cout << "\t Genre: "  << genre << endl;
	cout << "\t Year: "   << year << endl;

	for (int i = 0; i < 40; i++) cout << "-";
	cout << endl;
}