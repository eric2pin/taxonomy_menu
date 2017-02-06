(function ($) {

Backdrop.behaviors.taxonomyMenu = {
  // Provide the vertical tab summaries.
  attach: function (context) {
    var $context = $(context);

    // Status. List Disabled, or menu parent.
    $context.find('#edit-taxonomy-menu').backdropSetSummary(function(context) {
      var text = Backdrop.checkPlain($(context).find('select[name="taxonomy_menu[vocab_parent]"] option:selected').text());
      return text;
    });
  }
};

})(jQuery);
