#include "TBox.h"
#include "TText.h"
// #include "TLatex.h"
#include "TLine.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TColor.h"
#include "TSystem.h"
#include "TString.h"

#include<vector>

namespace sndisplay
{
  class calorimeter
  {
  public:
    calorimeter (const char *n = "") : name (n)
    {
      // canvas = NULL;
      canvas_it = NULL;
      canvas_fr = NULL;

      draw_omid = false;
      draw_omnum = false;
      draw_content = false;
      draw_content_format = "%.0f";

      has_italy_data = false;
      has_french_data = false;

      for (unsigned int omnum=0; omnum<nb_om; ++omnum)
	content.push_back(0);

      range_min = range_max = -1;

      const double spacerx = 0.0125;
      const double spacery = 0.0250;

      const double mw_sizey = (1-4*spacery)/(13+2);
      const double gv_sizey = mw_sizey;
      const double xw_sizey = mw_sizey*13./16.; // (1-4*spacery-2*gv_sizey)/16;

      const double mw_sizex = (1-4*spacerx)/(20+4); // (0.5-4*spacerx)/(20+4);
      const double gv_sizex = mw_sizex*20./16.;
      const double xw_sizex = mw_sizex;
      
      //////////////////////////
      // MWALL initialisation //
      //////////////////////////

      for (unsigned int mw_side=0; mw_side<2; ++mw_side) {

	for (unsigned int mw_column=0; mw_column<20; ++mw_column) {

	  for (unsigned int mw_row=0; mw_row<13; ++mw_row) {

	    unsigned int omnum = mw_side*20*13 + mw_column*13 + mw_row;

	    double x1 = spacerx + 2*xw_sizex + spacerx; // + 0.5*mw_side;
	    x1 += (mw_side == 0) ? mw_sizex*(19-mw_column) : mw_sizex*(mw_column); // swap IT for external view

	    double y1 = spacery + gv_sizey + spacery + mw_sizey*(mw_row);
	    double x2 = x1 + mw_sizex;
	    double y2 = y1 + mw_sizey;

	    TBox *box = new TBox(x1, y1, x2, y2);
	    box->SetFillColor(0);
	    box->SetLineWidth(1);
	    ombox.push_back(box);

	    TString omid_string = Form("M:%1d.%d.%d", mw_side, mw_column, mw_row);
	    TText *omid_text = new TText (x1+0.5*mw_sizex, y1+0.667*mw_sizey, omid_string);
	    omid_text->SetTextSize(0.014);
	    omid_text->SetTextAlign(22);
	    omid_text_v.push_back(omid_text);

	    TString omnum_string = Form("%03d", omnum);
	    TText *omnum_text = new TText (x1+0.5*mw_sizex, y1+0.333*mw_sizey, omnum_string);
	    omnum_text->SetTextSize(0.02);
	    omnum_text->SetTextAlign(22);
	    omnum_text_v.push_back(omnum_text);

	    TText *content_text = new TText (x1+0.5*mw_sizex, y1+0.333*mw_sizey, "");
	    content_text->SetTextSize(0.02);
	    content_text->SetTextAlign(22);
	    content_text_v.push_back(content_text);
  
	  } // for mw_row

	} // for mw_column

      } // for mw_side

      //////////////////////////
      // XWALL initialisation //
      //////////////////////////

      for (unsigned int xw_side=0; xw_side<2; ++xw_side) {

	for (unsigned int xw_wall=0; xw_wall<2; ++xw_wall) {

	  for (unsigned int xw_column=0; xw_column<2; ++xw_column) {

	    for (unsigned int xw_row=0; xw_row<16; ++xw_row) {

	    unsigned int omnum = 520 + xw_side*2*2*16 + xw_wall*2*16 + xw_column*16 + xw_row;

	    double x1;
	    
	    switch (xw_side) {
	    case 0:
	      if (xw_wall == 0) // IT-MO
		x1 = spacerx + 2*xw_sizex + spacerx + 20*mw_sizex + spacerx + (1-xw_column)*xw_sizex;
	      else              // IT-TU
		x1 = spacerx + xw_sizex*xw_column;
	      break;
	      
	    case 1:
	      if (xw_wall == 0) // FR-MO
		// x1 = 0.5 + spacerx + xw_sizex*xw_column;
		x1 = spacerx + xw_sizex*xw_column;
	      else // FR-TU
		// x1 = 0.5 + spacerx + 2*xw_sizex + spacerx + 20*mw_sizex + spacerx + (1-xw_column)*xw_sizex;
		x1 = spacerx + 2*xw_sizex + spacerx + 20*mw_sizex + spacerx + (1-xw_column)*xw_sizex;
	      break;}
	      
	    double x2 = x1 + xw_sizex;
	    double y1 = spacery + gv_sizey + spacery + xw_sizey*(xw_row);
	    double y2 = spacery + gv_sizey + spacery + xw_sizey*(xw_row+1);

	    TBox *box = new TBox(x1, y1, x2, y2);
	    box->SetFillColor(0);
	    box->SetLineWidth(1);
	    ombox.push_back(box);

	    TString omid_string = Form("X:%1d.%1d.%1d.%d", xw_side, xw_wall, xw_column, xw_row);
	    TText *omid_text = new TText (x1+0.5*mw_sizex, y1+0.6*mw_sizey, omid_string);
	    omid_text->SetTextSize(0.014);
	    omid_text->SetTextAlign(22);
	    omid_text_v.push_back(omid_text);

	    TString omnum_string = Form("%03d", omnum);
	    TText *omnum_text = new TText (x1+0.5*xw_sizex, y1+0.333*xw_sizey, omnum_string);
 	    omnum_text->SetTextSize(0.02);
 	    omnum_text->SetTextAlign(22);
	    omnum_text_v.push_back(omnum_text);

	    TText *content_text = new TText (x1+0.5*xw_sizex, y1+0.333*xw_sizey, "");
	    content_text->SetTextSize(0.02);
	    content_text->SetTextAlign(22);
	    content_text_v.push_back(content_text);

	    } // for xw_row

	  } // for xw_column

	} // for xw_wall

      } // for xw_side

      //////////////////////////
      // GVETO initialisation //
      //////////////////////////
      
      for (unsigned int gv_side=0; gv_side<2; ++gv_side) {

	for (unsigned int gv_wall=0; gv_wall<2; ++gv_wall) {

	  for (unsigned int gv_column=0; gv_column<16; ++gv_column) {

	    unsigned int omnum = 520 + 128 + gv_side*2*16 + gv_wall*16 + gv_column;

	    double x1;

	    if (gv_side == 0) 
	      x1 = spacerx + 2*xw_sizex + spacerx + gv_sizex*(16-1-gv_column);
	    else
	      // x1 = 0.5 + spacerx + 2*xw_sizex + spacerx + gv_sizex*gv_column;
	      x1 = spacerx + 2*xw_sizex + spacerx + gv_sizex*gv_column;

	    double x2 = x1 + gv_sizex;
	    double y1 = spacery + gv_wall*(gv_sizey + spacery + 13*mw_sizey + spacery);
	    double y2 = y1 + gv_sizey;
	    
	    TBox *box = new TBox(x1, y1, x2, y2);
	    box->SetFillColor(0);
	    box->SetLineWidth(1);
	    ombox.push_back(box);

	    TString omid_string = Form("G:%1d.%1d.%d", gv_side, gv_wall, gv_column);
	    TText *omid_text = new TText (x1+0.5*gv_sizex, y1+0.667*gv_sizey, omid_string);
	    omid_text->SetTextSize(0.014);
	    omid_text->SetTextAlign(22);
	    omid_text_v.push_back(omid_text);

	    TString omnum_string = Form("%03d", omnum);
	    TText *omnum_text = new TText (x1+0.5*gv_sizex, y1+0.333*gv_sizey, omnum_string);
	    omnum_text->SetTextSize(0.02);
	    omnum_text->SetTextAlign(22);
	    omnum_text_v.push_back(omnum_text);

	    TText *content_text = new TText (x1+0.5*gv_sizex, y1+0.333*gv_sizey, "");
	    content_text->SetTextSize(0.02);
	    content_text->SetTextAlign(22);
	    content_text_v.push_back(content_text);

	  } // for gv_column

	} // for gv_wall 

      } // for gv_side

      // source_foil = new TLine (0.5, spacery, 0.5, 1-spacery);
      // source_foil->SetLineWidth(2);

      it_label = new TText (spacerx,     spacery+gv_sizey+spacery+13*mw_sizey+spacery+0.25*gv_sizey, "  ITALY");
      it_label->SetTextSize(0.036);

      // fr_label = new TText (0.5 + spacerx, spacery+gv_sizey+spacery+13*mw_sizey+spacery+0.25*gv_sizey, "FRANCE");
      fr_label = new TText (spacerx, spacery+gv_sizey+spacery+13*mw_sizey+spacery+0.25*gv_sizey, "FRANCE");
      fr_label->SetTextSize(0.036);

      const Int_t nRGBs = 6;
      Double_t stops[nRGBs] = { 0.00, 0.20, 0.40, 0.60, 0.80, 1.00 };
      Double_t red[nRGBs]   = { 0.25, 0.00, 0.20, 1.00, 1.00, 0.90 };
      Double_t green[nRGBs] = { 0.25, 0.80, 1.00, 1.00, 0.80, 0.00 };
      Double_t blue[nRGBs]  = { 1.00, 1.00, 0.20, 0.00, 0.00, 0.00 };
      
      palette_index = TColor::CreateGradientColorTable(nRGBs, stops, red, green, blue, 100);

    };

    ~calorimeter() {};
    
    static const unsigned int nmwall = 520;
    static const unsigned int nxwall = 128;
    static const unsigned int ngveto =  64;

    static const unsigned int nb_om  = 712;

    enum {
      AUTO,
      FULL,
      FULL_ITALY,
      FULL_FRANCE,
      MW_ITALY,
      MW_FRANCE,
      XW_MOUNTAIN,
      XW_TUNNEL};

    void setrange(float xmin, float xmax) 
    {
      range_min = xmin; range_max = xmax;
    }

    void draw_omid_label() {
      draw_omid = true;}

    void draw_omnum_label() {
      draw_omnum = true;}
 
    void draw_content_label(const char *format="%.0f") {
      draw_content_format = TString(format);
      draw_content = true;}

    void draw()
    {
      if (canvas_it == NULL)
	// canvas_it = new TCanvas (Form("C_it_%s",name.Data()), name, 1400, 800);
	canvas_it = new TCanvas (Form("C_it_%s",name.Data()), name, 900, 600);

      if (canvas_fr == NULL)
	canvas_fr = new TCanvas (Form("C_fr_%s",name.Data()), name, 900, 600);

      // if (has_french_data && (canvas_fr == NULL))
      // 	canvas_fr = new TCanvas (Form("C_it_%s",name.Data()), name, 1400, 800);

      // if (pad_italy != NULL) delete pad_italy;
      // pad_italy = new TPad ();
      // canvas->cd();
      // canvas->SetEditable(true);      

      if (draw_content)
	{
	  for (int omnum=0; omnum<nb_om; ++omnum)
	    {
	      TText *ttext = content_text_v[omnum];
	      ttext->SetText(ttext->GetX(), ttext->GetY(), Form(draw_content_format.Data(), content[omnum]));
	    }
	}

      /////////////
      // Draw IT //
      /////////////

      canvas_it->cd();
      canvas_it->SetEditable(true);
      
      unsigned int mw_side=0;
      for (unsigned int mw_column=0; mw_column<20; ++mw_column) {
	for (unsigned int mw_row=0; mw_row<13; ++mw_row) {
	  unsigned int id = mw_side*20*13 + mw_column*13 + mw_row;
	  ombox[id]->Draw("l");
	  if (draw_omid) // if (((mw_column % 5) == 0) || (mw_column == 19))
	    omid_text_v[id]->Draw();
	  if (draw_omnum) omnum_text_v[id]->Draw();
	  if (draw_content) content_text_v[id]->Draw();
	  // else if (draw_omid_num) omid_num_text_v[id]->Draw();
	}
      }
      
      unsigned int xw_side=0;
      for (unsigned int xw_wall=0; xw_wall<2; ++xw_wall) {
	for (unsigned int xw_column=0; xw_column<2; ++xw_column) {
	  for (unsigned int xw_row=0; xw_row<16; ++xw_row) {
	    unsigned int id = 520 + xw_side*2*2*16 + xw_wall*2*16 + xw_column*16 + xw_row;
	    ombox[id]->Draw("l");
	    if (draw_omid) // if ((xw_column % 5) == 0)
	      omid_text_v[id]->Draw();
	    if (draw_omnum) omnum_text_v[id]->Draw();
	    if (draw_content) content_text_v[id]->Draw();
	    // else if (draw_omid_num) omid_num_text_v[id]->Draw();
	  }
	}
      }
      
      unsigned int gv_side=0;
      for (unsigned int gv_wall=0; gv_wall<2; ++gv_wall) {
	for (unsigned int gv_column=0; gv_column<16; ++gv_column) {
	  unsigned int id = 520 + 128 + gv_side*2*16 + gv_wall*16 + gv_column;
	  ombox[id]->Draw("l");
	  if (draw_omid) // if ((gv_column % 5) == 0)
	    omid_text_v[id]->Draw();
	  if (draw_omnum) omnum_text_v[id]->Draw();
	  if (draw_content) content_text_v[id]->Draw();
	  // else if (draw_omid_num) omid_num_text_v[id]->Draw();
	}
      }

      it_label->Draw();
      
      canvas_it->SetEditable(false);

      /////////////
      // Draw FR //
      /////////////

      canvas_fr->cd();
      canvas_fr->SetEditable(true);
      
      mw_side=1;
      for (unsigned int mw_column=0; mw_column<20; ++mw_column) {
	for (unsigned int mw_row=0; mw_row<13; ++mw_row) {
	  unsigned int id = mw_side*20*13 + mw_column*13 + mw_row;
	  ombox[id]->Draw("l");
	  if (draw_omid) // if (((mw_column % 5) == 0) || (mw_column == 19))
	      omid_text_v[id]->Draw();
	  if (draw_omnum) omnum_text_v[id]->Draw();
	  if (draw_content) content_text_v[id]->Draw();
	  // else if (draw_omid_num) omid_num_text_v[id]->Draw();
	}
      }
      
      xw_side=1;
      for (unsigned int xw_wall=0; xw_wall<2; ++xw_wall) {
	for (unsigned int xw_column=0; xw_column<2; ++xw_column) {
	  for (unsigned int xw_row=0; xw_row<16; ++xw_row) {
	    unsigned int id = 520 + xw_side*2*2*16 + xw_wall*2*16 + xw_column*16 + xw_row;
	    ombox[id]->Draw("l");
	    if (draw_omid) // if ((xw_column % 5) == 0)
	      omid_text_v[id]->Draw();
	    if (draw_omnum) omnum_text_v[id]->Draw();
	    if (draw_content) content_text_v[id]->Draw();
	    // else if (draw_omid_num) omid_num_text_v[id]->Draw();
	  }
	}
      }
      
      gv_side=1;
      for (unsigned int gv_wall=0; gv_wall<2; ++gv_wall) {
	for (unsigned int gv_column=0; gv_column<16; ++gv_column) {
	  unsigned int id = 520 + 128 + gv_side*2*16 + gv_wall*16 + gv_column;
	  ombox[id]->Draw("l");
	  if (draw_omid) // if ((gv_column % 5) == 0)
	    omid_text_v[id]->Draw();
	  if (draw_omnum) omnum_text_v[id]->Draw();
	  if (draw_content) content_text_v[id]->Draw();
	  // else if (draw_omid_num) omid_num_text_v[id]->Draw();
	}
      }

      fr_label->Draw();
      canvas_fr->SetEditable(false);

      //

      update();
    }

    
    void reset() {
      for (unsigned int omnum=0; omnum<nb_om; ++omnum)
	content[omnum] = 0;

      // float content_min = 0; float content_max = 1;

      for (unsigned int omnum=0; omnum<nb_om; ++omnum)
	ombox[omnum]->SetFillColor(0);
      
      canvas_it->Modified();
      canvas_it->Update();
      gSystem->ProcessEvents();
      
    }


    void setcontent (int omnum, float value)
    {
      if (omnum>=0 && omnum<260)
	has_italy_data = true;
      else if (omnum<520)
	has_french_data = true;
      else if (omnum<584)
	has_italy_data = true;
      else if (omnum<648)
	has_french_data = true;
      else if (omnum<680)
	has_italy_data = true;
      else if (omnum<712)
	has_french_data = true;
      
      content[omnum] = value;
    }
    
    void setcontent (int om_side, int om_wall, int om_column, int om_row, float value)
    {
      int omnum = -1;

      // auto detect MW
      if ((om_side!=-1) && (om_wall==-1) && (om_column!=-1) && (om_row!=-1))
	omnum = 260*om_side + 13*om_column + om_row;

      // auto detect XW
      else if ((om_side!=-1) && (om_wall!=-1) && (om_column!=-1) && (om_row!=-1))
	omnum = 520 + 64*om_side + 32*om_wall + 16*om_column + om_row;
       
      // auto detect GV
     else if ((om_side!=-1) && (om_wall!=-1) && (om_column!=-1) && (om_row==-1))
	omnum = 520 + 128 + 32*om_side + 16*om_wall + om_column;

      else {
	printf("+++ sndisplay: skipping OM (%d.%d.%d.%d)\n", om_side, om_wall, om_column, om_row);
	return;}

      if (om_side == 0)
	has_italy_data = true;
      else if (om_side == 1)
	has_french_data = true;

      content[omnum] = value;
    }

    
    void fill (int omnum, float value=1)
    {
      setcontent(omnum, content[omnum]+value);
    }

    // void fill (int om_side, int om_wall, int om_column, int om_row, float value=1)
    // {
    //   setcontent(om_side, om_wall, om_column, om_row, content[omnum]+value);
    // }

    void update()
    {
      float content_min = content[0];
      float content_max = content[0];

      if (range_min == -1) {
	for (unsigned int omnum=1; omnum<nb_om; ++omnum) {
	  if (content[omnum] < content_min) content_min = content[omnum];
	  if (content[omnum] > content_max) content_max = content[omnum];}
      } else {
	range_min = 0; content_max = range_max;}

      for (unsigned int omnum=0; omnum<nb_om; ++omnum)
	if (content[omnum] != 0)
	  // ombox[omnum]->SetFillColor(TColor::GetColorPalette((int)(99*(content[omnum]-content_min)/(content_max-content_min))));
	  ombox[omnum]->SetFillColor(palette_index + (int)(99*(content[omnum]-content_min)/(content_max-content_min)));
	else
	  ombox[omnum]->SetFillColor(0);

      canvas_it->Modified();
      canvas_it->Update();
      gSystem->ProcessEvents();
    }
    

    // void setcontent(geomid) 

    // void setcontent (uint32_t geomid0, uint32_t geomid1, uint32_t geomid2, uint32_t geomid3, uint32_t geomid4, float content)
    // {
    //   switch (geomid0)
    // 	{
    // 	  case 
    // 	}
    // }

    TString name;

    float range_min, range_max;

    std::vector<float> content;
    std::vector<TBox*>   ombox;

    bool draw_omid;
    bool draw_omnum;
    bool draw_content;
    TString draw_content_format;

    bool has_italy_data;
    bool has_french_data;

    std::vector<TText*> omid_text_v;
    std::vector<TText*> omnum_text_v;
    std::vector<TText*> content_text_v;

    TLine *source_foil;
    TText *it_label;
    TText *fr_label;
    
    // TCanvas *canvas;
    TCanvas *canvas_it;
    TCanvas *canvas_fr;

    TPad *pad_italy;
    TPad *pad_french;

    int palette_index;
  };
}

void sndisplay_test ()
{
  sndisplay::calorimeter *sncalo = new sndisplay::calorimeter;

  sncalo->draw_omid_label();
  // sncalo->draw_omnum_label();
  sncalo->draw_content_label("%.1f");

  // sncalo->setcontent(0, 0.001);
  // for (int omnum=1; omnum<712; ++omnum)
  //   sncalo->setcontent(omnum, omnum/7.11);
  //   // sncalo->ombox[omnum]->SetFillColor(TColor::GetColorPalette((int)(omnum/7.13)));

  TRandom trand;
  
  for (int omnum=0; omnum<520; ++omnum)
    sncalo->setcontent(omnum, trand.Gaus(100, 10));
  
  for (int omnum=520; omnum<712; ++omnum)
    sncalo->setcontent(omnum, trand.Gaus(50, 10));
		       
  
  // sncalo->setcontent(0, -1, 0,  0, 1); // M:0.0.0
  // sncalo->setcontent(0,  0, 0,  0, 1); // X:0.0.0.0
  // sncalo->setcontent(0,  0, 0, -1, 1); // G:0.0.0

  // sncalo->setcontent(0, -1, 0,  1, 2); // M:0.0.1
  // sncalo->setcontent(0,  0, 0,  1, 2); // X:0.0.0.1
  // sncalo->setcontent(0,  0, 1, -1, 2); // G:0.0.1


  // sncalo->setcontent(1, -1, 19, 11, 3); // M:1.19.11
  // sncalo->setcontent(1,  1,  1, 14, 3); // X:1.1.1.14
  // sncalo->setcontent(1,  1, 14, -1, 3); // G:1.1.14

  // sncalo->setcontent(1, -1, 19, 12, 4); // M:1.19.12
  // sncalo->setcontent(1,  1,  1, 15, 4); // X:1.1.1.15
  // sncalo->setcontent(1,  1, 15, -1, 4); // G:1.1.15

  sncalo->draw();

}

