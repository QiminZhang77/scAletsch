/*
Part of Scallop Transcript Assembler
(c) 2017 by  Mingfu Shao, Carl Kingsford, and Carnegie Mellon University.
See LICENSE for licensing.
*/

#include "item.h"

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <cassert>
#include <cstdio>
#include <cmath>

item::item(const string &s)
{
	parse(s);
}

int item::parse(const string &s)
{
	char buf[10240];
	stringstream sstr(s);
	sstr>>buf;
	seqname.assign(buf);
	sstr>>buf;
	source.assign(buf);
	sstr>>buf;
	feature.assign(buf);
	sstr>>start>>end;
	start--;			// TODO gtf: (from 1, both inclusive)
	sstr>>buf;
	if(buf[0] == '.') score = -1;
	else score = atof(buf);
	sstr>>buf;
	strand = buf[0];
	sstr>>buf;
	frame = buf[0];

	char buf2[10240];
	coverage = 0;
	RPKM = 0;
	TPM = 0;
	while(sstr.eof() == false)
	{
		sstr>>buf;
		/*
		sstr.getline(buf2, 10240, '"');
		sstr.getline(buf2, 10240, '"');
		*/
		sstr.getline(buf2, 10240, ';');
		string v(buf2);
		int k1 = v.find('"');
		int k2 = v.rfind('"');
		if(k1 >= 0 && k1 < k2 && k2 < v.size()) v = v.substr(k1 + 1, k2 - k1 - 1);

		if(string(buf) == "" || v == "") break;

		//printf("|%s|%s|\n", buf, v.c_str());

		if(string(buf) == "transcript_id") transcript_id = seqname + "." + v;
		else if(string(buf) == "transcript_biotype") transcript_type = v;
		else if(string(buf) == "gene_biotype") gene_type = v;
		else if(string(buf) == "gene_id") gene_id = v;
		else if(string(buf) == "y_prob") coverage = atof(v.c_str());
		//else if(string(buf) == "cov2") coverage = atof(v.c_str());
		//else if(string(buf) == "cov") coverage = atof(v.c_str());
		//else if(string(buf) == "coverage") coverage = atof(v.c_str());
		else if(string(buf) == "expression") coverage = atof(v.c_str());
		else if(string(buf) == "expr") coverage = atof(v.c_str());
		else if(string(buf) == "TPM") TPM = atof(v.c_str());
		else if(string(buf) == "RPKM") RPKM = atof(v.c_str());
		else if(string(buf) == "FPKM") FPKM = atof(v.c_str());
		else if(string(buf) == "empty_vertex_list") 
		{
			char buf3[10240];
			stringstream snfp(v);
			while(snfp.eof() == false)
			{
				snfp.getline(buf3, 10240, '.');
				string p(buf3);
				p = p.substr(1, p.length()-1);
				stringstream sp(p);
				char buf4[10240];
				sp.getline(buf4, 10240, ',');
				string lp(buf4);
				int lpos = atoi(lp.c_str());
				sp.getline(buf4, 10240, ',');
				string rp(buf4);
				int rpos = atoi(rp.c_str());
				vector<int> pp;
				pp.push_back(lpos);
				pp.push_back(rpos);
				nfp.push_back(pp);
			}

		}
		//sstr.getline(buf2, 10240, ';');
	}

	return 0;
}

int item::print() const
{
	printf("%s\t%s\t%s\t%d\t%d\t%.1lf\t%c\t%c\ttranscript_id \"%s\"; gene_id \"%s\"; coverage \"%.2f\"; RPKM \"%.2lf\"\n",
			seqname.c_str(), source.c_str(), feature.c_str(), start, end, score, strand, frame,
			transcript_id.c_str(), gene_id.c_str(), coverage, RPKM);
	return 0;
}

bool item::operator<(const item &ge) const
{
	if(start < ge.start) return true;
	else return false;
}

int item::length() const
{
	assert(end > start);
	return end - start;
}
