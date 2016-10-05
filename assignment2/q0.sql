CREATE TABLE poi (
	id INT,
	uid INT,
	lat FLOAT,
	lon FLOAT,
	PRIMARY KEY(id)
);

CREATE TABLE poi_tag (
	id INT,
	key TEXT,
	value TEXT
);

.mode tsv
.import poi_tag.tsv poi_tag
.import poi.tsv poi